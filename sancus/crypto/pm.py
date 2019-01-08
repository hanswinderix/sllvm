import ccrypto

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
    assert t4, self.name
    STVAL = 'st_value'
    return (t1[STVAL], t2[STVAL], t3[STVAL], t4[STVAL])
    
  def compute_identity(self):
    identity = self.get_text()
    identity += self.get_text_start().to_bytes(2, byteorder='little')
    identity += self.get_text_end().to_bytes(2, byteorder='little')
    identity += self.get_data_start().to_bytes(2, byteorder='little')
    identity += self.get_data_end().to_bytes(2, byteorder='little')
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

  def get_hash(self, size):
    return ccrypto.compute_sancus_hash(size, self.get_identity())

  def get_mac(self, key):
    return ccrypto.compute_sancus_mac(key, self.get_identity())
