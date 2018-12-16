#!/usr/bin/env python3

import sys
import re

from elftools.elf.elffile import ELFFile

#############################################################################
class PM:
  
  def __init__(self, elf, name):
    self.elf = elf
    self.elf_text_section = elf.get_section_by_name(".text")
    assert self.elf_text_section
    self.name = name
    self.set_layout()
    #self.init_macs_for_secure_linking()

  def set_layout(self):
    t1 = find_ELF_symbol(self.elf, 'sllvm_pm_%s_text_start' % self.name)
    assert t1
    t2 = find_ELF_symbol(self.elf, 'sllvm_pm_%s_text_end' % self.name)
    assert t2
    t3 = find_ELF_symbol(self.elf, 'sllvm_pm_%s_data_start' % self.name)
    assert t3
    t4 = find_ELF_symbol(self.elf, 'sllvm_pm_%s_data_end' % self.name)
    assert t4
    STVAL = 'st_value'
    self.layout = (t1[STVAL], t2[STVAL], t3[STVAL], t4[STVAL])

  def text(self):
    start = self.get_text_offset_in_ELF_text_section()
    stop = start + self.get_text_size()
    return self.elf_text_section.data()[start:stop]

  #def init_macs_for_secure_linking(self):
  # for match, smac in match_ELF_symbols(elf, 'sllvm_mac_%s_(\w+)' % self.name):
  #    sf = find_ELF_symbol(elf, match.group(1))
  #    assert sf

  def get_text_start(self):
    return self.layout[0]

  def get_text_end(self):
    return self.layout[1]

  def get_text_size(self):
    size = self.get_text_end() - self.get_text_start()
    assert size > 0
    sh_size = self.elf_text_section['sh_size']
    sh_addr = self.elf_text_section['sh_addr']
    assert self.get_text_offset_in_ELF_text_section() + size <= \
        sh_addr + sh_size
    return size

  def get_text_offset_in_ELF_text_section(self):
    sh_addr = self.elf_text_section['sh_addr']
    sh_size = self.elf_text_section['sh_size']
    offset = self.get_text_start() - sh_addr
    assert offset > 0
    return offset

  def get_data_start(self):
    return self.layout[2]

  def get_data_end(self):
    return self.layout[3]

#############################################################################
def find_ELF_symbol(elf, name):
  for section in elf.iter_sections():
    if section.header['sh_type'] == 'SHT_SYMTAB':
      for sym in section.iter_symbols():
        if sym.name == name:
          return sym
  return None

#############################################################################
def match_ELF_symbols(elf, pattern):
  matches = []
  for section in elf.iter_sections():
    if section.header['sh_type'] == 'SHT_SYMTAB':
      for sym in section.iter_symbols():
        m = re.match(pattern, sym.name)
        if m:
          matches.append((m, sym))
  return matches

#############################################################################
def extract_protected_modules_from_ELF(elf):
  pms = []
  for match, symbol in match_ELF_symbols(elf, 'sllvm_pm_(\w+)_text_start'):
    pms.append(PM(elf, match.group(1)))
  return pms

#############################################################################

assert len(sys.argv) > 1
elf = ELFFile(open(sys.argv[1], 'rb')) 
pms = extract_protected_modules_from_ELF(elf)
for pm in pms:
  print(pm.name)
  print(pm.text().hex())
