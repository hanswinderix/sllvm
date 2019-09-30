import re

from elftools.elf.elffile import ELFFile

class ELF:

  def __init__(self, fname):
    self.name = fname
    self.elf = ELFFile(open(fname, 'rb'))

  def get_name(self):
    return self.name

  def find_section_by_address(self, addr):
    for section in self.elf.iter_sections():
      sh_addr = section['sh_addr']
      sh_size = section['sh_size']
      if (addr >= sh_addr) and (addr < (sh_addr + sh_size)):
        return section
    return None

  def find_symbol_by_name(self, name):
    for section in self.elf.iter_sections():
      if section.header['sh_type'] == 'SHT_SYMTAB':
        for sym in section.iter_symbols():
          if sym.name == name:
            return sym
    return None

  def find_symbol_by_address(self, addr):
    for section in self.elf.iter_sections():
      if section.header['sh_type'] == 'SHT_SYMTAB':
        for sym in section.iter_symbols():
          # TODO: Relax condition
          #         (addr should be within range [st_value, stvalue+size[)
          if sym['st_value'] == addr:
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

  def extract_bytes(self, symbol):
    section = self.find_section_by_address(symbol['st_value'])
    assert section
    offset = symbol['st_value'] - section['sh_addr']
    #offset_in_elf = section['sh_offset'] + offset
    # TODO: Check that result is within bounds
    return section.data()[offset:offset+symbol['st_size']]

  ###########################################################################
  # Text section specific function
  # TODO: Replace them with more generic ones

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
