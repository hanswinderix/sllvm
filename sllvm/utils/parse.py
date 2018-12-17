#!/usr/bin/env python3

import sys
import re
import ctypes
import shutil

from elftools.elf.elffile import ELFFile

#############################################################################
# TODO: Don't hardcode these config values
KEY_BITSIZE = 64
KEY_BYTESIZE = ((KEY_BITSIZE + 7) // 8)
libname = '/usr/local/share/sancus-compiler/libsancus-crypto.so'

_lib = ctypes.cdll.LoadLibrary(libname)

def compute_sancus_mac(key, msg):
  mac = bytes(KEY_BYTESIZE)
  clen = ctypes.c_ulonglong(len(msg))
  _lib.sancus_mac(key, msg, clen, mac)
  return mac

#############################################################################

class ELF:

  def __init__(self, fname):
    self.name = fname
    self.elf = ELFFile(open(fname, 'rb')) 

  def get_name(self):
    return self.name

  def find_symbol_by_name(self, name):
    for section in self.elf.iter_sections():
      if section.header['sh_type'] == 'SHT_SYMTAB':
        for sym in section.iter_symbols():
          if sym.name == name:
            return sym
    return None

  def match_symbols(self, pattern):
    matches = []
    for section in self.elf.iter_sections():
      if section.header['sh_type'] == 'SHT_SYMTAB':
        for sym in section.iter_symbols():
          m = re.match(pattern, sym.name)
          if m:
            matches.append((m, sym))
    return matches

  def get_text_section(self):
    section = self.elf.get_section_by_name(".text")
    assert section
    return section

  def get_offset_in_text_section(self, addr):
    section = self.get_text_section()
    sh_addr = section['sh_addr']
    assert addr > sh_addr
    offset = addr - sh_addr
    return offset

  def get_offset_of_text_addr_in_file(self, addr):
    section = self.get_text_section()
    offset = self.get_offset_in_text_section(addr)
    sh_offset = section['sh_offset']
    # TODO: Check that result is within bounds
    return sh_offset + offset

  def extract_text(self, addr, size):
    section = self.get_text_section()
    sh_size = section['sh_size']
    sh_addr = section['sh_addr']
    assert addr + size <= sh_addr + sh_size
    start = self.get_offset_in_text_section(addr)
    stop = start + size
    return self.get_text_section().data()[start:stop]

#############################################################################
class PM:
  
  def __init__(self, elf, name):
    self.elf = elf
    self.name = name
    self.layout = None
    self.identity = None

  def compute_layout(self):
    t1 = self.elf.find_symbol_by_name('sllvm_pm_%s_text_start' % self.name)
    assert t1
    t2 = self.elf.find_symbol_by_name('sllvm_pm_%s_text_end' % self.name)
    assert t2
    t3 = self.elf.find_symbol_by_name('sllvm_pm_%s_data_start' % self.name)
    assert t3
    t4 = self.elf.find_symbol_by_name('sllvm_pm_%s_data_end' % self.name)
    assert t4
    STVAL = 'st_value'
    return (t1[STVAL], t2[STVAL], t3[STVAL], t4[STVAL])
    
  def compute_identity(self):
    identity = self.get_text()
    identity += self.get_text_start().to_bytes(2, byteorder='big')
    identity += self.get_text_end().to_bytes(2, byteorder='big')
    identity += self.get_data_start().to_bytes(2, byteorder='big')
    identity += self.get_data_end().to_bytes(2, byteorder='big')
    return identity

  def has_symbol(self, name):
    sym = self.elf.find_symbol_by_name(name)
    assert sym
    addr = sym['st_value']
    return addr >= self.get_text_start() and addr < self.get_text_end()
    # TODO: Make sure that the end is also within the pm boundaries

  def get_text_start(self):
    return self.get_layout()[0]

  def get_text_end(self):
    return self.get_layout()[1]

  def get_text_size(self):
    return self.get_text_end() - self.get_text_start()

  def get_data_start(self):
    return self.get_layout()[2]

  def get_data_end(self):
    return self.get_layout()[3]

  def get_text(self):
    return self.elf.extract_text(self.get_text_start(), self.get_text_size())

  def get_layout(self):
    if self.layout == None:
      self.layout = self.compute_layout()
    return self.layout

  def get_name(self):
    return self.name

  def get_identity(self):
    if self.identity == None:
      self.identity = self.compute_identity()
    return self.identity

  def get_mac(self, key):
    return compute_sancus_mac(key, self.get_identity())

  def get_hash(self):
    return self.get_mac(bytes(KEY_BYTESIZE))

#############################################################################
class Loader:

  def __init__(self, fname):
    self.elf = ELF(fname)
    assert self.elf
    self.PMs = []
    for match, _ in self.elf.match_symbols('sllvm_pm_(\w+)_text_start'):
      self.PMs.append(PM(self.elf, match.group(1)))

  def get_protected_modules(self):
    return self.PMs

  def find_protected_module_by_symbol_name(self, name):
    l = [pm for pm in self.get_protected_modules() if pm.has_symbol(name)]
    if l:
      assert len(l) == 1
      return l[0]
    return None

  # Required for secure linking
  def fill_hashes(self, fname):
    shutil.copy(self.elf.get_name(), fname)
    with open(fname, 'rb+') as f:
      for pm1 in self.get_protected_modules():
        pattern = 'sllvm_hash_%s_(\w+)' % pm1.get_name()
        for match, sym in self.elf.match_symbols(pattern):
          pm2 = self.find_protected_module_by_symbol_name(match.group(1))
          assert pm2
          assert pm1 != pm2
          hash = pm2.get_hash()
          size = sym['st_size']
          assert size == KEY_BYTESIZE
          assert size == len(hash)
          offset = self.elf.get_offset_of_text_addr_in_file(sym['st_value'])
          f.seek(offset)
          f.write(hash)

#############################################################################
assert len(sys.argv) > 2
l = Loader(sys.argv[1])
l.fill_hashes(sys.argv[2])
