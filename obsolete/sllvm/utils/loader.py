import elf
import pm

class Loader:

  def __init__(self, fname):
    self.elf = elf.ELF(fname)
    assert self.elf
    self.PMs = []
    for match, _ in self.elf.match_symbols('sllvm_pm_(\w+)_text_start'):
      self.PMs.append(pm.PM(self.elf, match.group(1)))

  def get_ELF(self):
    return self.elf

  def get_protected_modules(self):
    return self.PMs

  def find_protected_module_by_name(self, name):
    return [x for x in self.PMS if x.get_name() == name]

  def find_protected_module_by_symbol_name(self, name):
    l = [pm for pm in self.get_protected_modules() if pm.has_symbol(name)]
    if l:
      assert len(l) == 1
      return l[0]
    return None
