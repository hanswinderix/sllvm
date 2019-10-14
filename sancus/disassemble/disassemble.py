import sys
import loader
import io

def read_word(stream):
  w = stream.read(2)
  if w: return (w[1] << 8) | w[0]
  return None

#############################################################################

class InstructionFactory:

  def create(self, word1):
    form = word1 & 0xf000
    if   form in [0x1000]        : return FormatII()
    elif form in [0x2000, 0x3000]: return Jump()
    else                         : return FormatI()

class Instruction:

  def parse(self, stream, word1):
    assert False, "Abstract method needs to be implemented"

  def __str__(self):
    assert False, "Abstract method needs to be implemented"

class FormatI(Instruction):

  def parse(self, stream, word1):

    self.word1 = word1

    self.Opcode = (word1 & 0xf000) >> 12
    self.SReg   = (word1 & 0x0f00) >>  8
    self.Ad     = (word1 & 0x0080) >>  7
    self.BW     = (word1 & 0x0040) >>  6
    self.As     = (word1 & 0x0030) >>  4
    self.DReg   = (word1 & 0x000f)

    self.length  = 0xFF
    self.latency = 0xFF

    if   (word1 & 0xf000) == 0x4000: self.mnemonic = "MOV"
    elif (word1 & 0xf000) == 0x5000: self.mnemonic = "ADD"
    elif (word1 & 0xf000) == 0x6000: self.mnemonic = "ADDC"
    elif (word1 & 0xf000) == 0x7000: self.mnemonic = "SUBC"
    elif (word1 & 0xf000) == 0x8000: self.mnemonic = "SUB"
    elif (word1 & 0xf000) == 0x9000: self.mnemonic = "CMP"
    elif (word1 & 0xf000) == 0xa000: self.mnemonic = "DADD"
    elif (word1 & 0xf000) == 0xb000: self.mnemonic = "BIT"
    elif (word1 & 0xf000) == 0xc000: self.mnemonic = "BIC"
    elif (word1 & 0xf000) == 0xd000: self.mnemonic = "BIS"
    elif (word1 & 0xf000) == 0xe000: self.mnemonic = "XOR"
    elif (word1 & 0xf000) == 0xf000: self.mnemonic = "AND"
    else: assert False, hex(word1)

    if ( (self.SReg == 0x02) and (self.As > 2) ) or (self.SReg == 0x03):
      self.length  = 1
      self.latency = 1
      if self.Ad == 0x01:
        self.length  = self.length + 1
        self.latency = self.length + 2
    else:
      if   (self.As, self.Ad) == (0x00, 0x00):
        self.latency = 1
        if self.DReg == 0x00: self.latency = self.latency + 1
        self.length  = 1
      elif (self.As, self.Ad) == (0x00, 0x01):
        self.latency = 4
        self.length  = 2

      elif (self.As, self.Ad) == (0x01, 0x00):
        self.latency = 3
        self.length  = 2
      elif (self.As, self.Ad) == (0x01, 0x01):
        self.latency = 6
        self.length  = 3

      elif (self.As, self.Ad) == (0x02, 0x00):
        self.latency = 2
        self.length  = 1
      elif (self.As, self.Ad) == (0x02, 0x01):
        self.latency = 5
        self.length  = 2
      elif (self.As, self.Ad) == (0x03, 0x00):
        self.latency = 2
        if self.DReg == 0x00: self.latency = self.latency + 1
        self.length  = 1
        if self.SReg == 0x00: self.length = self.length + 1
      elif (self.As, self.Ad) == (0x03, 0x01):
        self.latency = 5
        self.length  = 2
        if self.SReg == 0x00: self.length = self.length + 1

    # Consume rest of instruction
    if self.length > 1:
      self.word2 = read_word(stream)
      if self.length > 2:
        self.word3 = read_word(stream)

  def __str__(self):
    if   (self.As, self.Ad) == (0x00, 0x00):
      return "%s R%d, R%d" % (self.mnemonic, self.SReg, self.DReg)
    elif (self.As, self.Ad) == (0x00, 0x01):
      return "%s R%d, %d(R%d)" % \
            (self.mnemonic, self.SReg, self.word2, self.DReg)
    elif (self.As, self.Ad) == (0x01, 0x00):
      if self.SReg == 0x03:
        return "%s 0(R%d), R%d" % \
            (self.mnemonic, self.SReg, self.DReg)
      else:
        return "%s %d(R%d), R%d" % \
          (self.mnemonic, self.word2, self.SReg, self.DReg)
    elif (self.As, self.Ad) == (0x01, 0x01):
      return "%s %d(R%d), %d(R%d)" % \
            (self.mnemonic, self.word2, self.SReg, self.word3, self.DReg)
    elif (self.As, self.Ad) == (0x02, 0x00):
      return "%s @R%d, R%d" % (self.mnemonic, self.SReg, self.DReg)
    elif (self.As, self.Ad) == (0x02, 0x01):
      return "%s @R%d, %d(R%d)" % \
          (self.mnemonic, self.SReg, self.word2, self.DReg)
    elif (self.As, self.Ad) == (0x03, 0x00):
      return "%s @R%d+, R%d" % (self.mnemonic, self.SReg, self.DReg)
    elif (self.As, self.Ad) == (0x03, 0x01):
      return "%s @R%d+, %d(R%d)" % \
          (self.mnemonic, self.SReg, self.word2, self.DReg)
    else: assert False, (self.As, self.Ad)

class FormatII(Instruction):

  def parse(self, stream, word1):

    self.word1 = word1

    self.Opcode = (word1 & 0xff80) >> 7
    self.BW     = (word1 & 0x0040) >> 6
    self.Ad     = (word1 & 0x0030) >> 4
    self.DSReg  = (word1 & 0x000f)

    self.length  = 0xFF
    self.latency = 0xFF

    if (word1 & 0xff80) == 0x1300:
      self.mnemonic = "RETI"
      self.length   = 1
      self.latency  = 5
    else:
      # PUSH
      if   (word1 & 0xff80) == 0x1200:
        self.mnemonic = "PUSH"
        if   self.Ad == 0x00: self.latency = 3
        elif self.Ad == 0x01: self.latency = 5
        elif self.Ad == 0x02: self.latency = 4
        elif self.Ad == 0x03:
          self.latency = 5
          if self.DSReg == 0x00: self.latency = 4
        else: assert False, self.Ad
      # CALL
      elif (word1 & 0xff80) == 0x1280:
        self.mnemonic = "CALL"
        if   self.Ad == 0x00: self.latency = 4
        elif self.Ad == 0x01: self.latency = 5
        elif self.Ad == 0x02: self.latency = 4
        elif self.Ad == 0x03: self.latency = 5
        else: assert False, self.Ad
      # RRA, RRC, SWPB, SXT
      else:
        if   (word1 & 0xff80) == 0x1000: self.mnemonic = "RRC"
        elif (word1 & 0xff80) == 0x1080: self.mnemonic = "SWPB"
        elif (word1 & 0xff80) == 0x1100: self.mnemonic = "RRA"
        elif (word1 & 0xff80) == 0x1180: self.mnemonic = "SXT"
        else: assert False, hex(word1)

        if   self.Ad == 0x00: self.latency = 4
        elif self.Ad == 0x01: self.latency = 5
        elif self.Ad == 0x02: self.latency = 4
        elif self.Ad == 0x03: self.latency = 5
        else: assert False, self.Ad

      self.length = 1
      if (self.Ad == 0x01) or ( (self.Ad == 0x03) and (self.DSReg == 0x00) ):
        self.length = 2

    # Consume rest of instruction
    if self.length > 1:
      assert self.length == 2
      self.word2 = read_word(stream)

  def __str__(self):
    if   self.Ad == 0x00:
      return "%s R%d" % (self.mnemonic, self.DSReg)
    elif self.Ad == 0x01:
      return "%s %d(R%d)" % (self.mnemonic, self.word2, self.DSReg)
    elif self.Ad == 0x02:
      return "%s @R%d" % (self.mnemonic, self.DSReg)
    elif self.Ad == 0x03:
      if self.DSReg == 0x00:
        return "%s #0x%x" % (self.mnemonic, self.word2)
      else:
        return "%s @R%d+" % (self.mnemonic, self.DSReg)
    else: assert False, self.Ad

class Jump(Instruction):

  def parse(self, stream, word1):

    self.word1 = word1

    self.Opcode = (word1 & 0xe000) >> 13
    self.C      = (word1 & 0x1c00) >> 10
    self.Offset = (word1 & 0x03ff)
    if (word1 & 0x0200) == 0x0200:
      self.Offset = self.Offset - 1024

    self.length  = 1
    self.latency = 2

    if   (word1 & 0xfc00) == 0x2000: self.mnemonic = "JNE"
    elif (word1 & 0xfc00) == 0x2400: self.mnemonic = "JEQ"
    elif (word1 & 0xfc00) == 0x2800: self.mnemonic = "JNC"
    elif (word1 & 0xfc00) == 0x2C00: self.mnemonic = "JC"
    elif (word1 & 0xfc00) == 0x3000: self.mnemonic = "JN"
    elif (word1 & 0xfc00) == 0x3400: self.mnemonic = "JGE"
    elif (word1 & 0xfc00) == 0x3800: self.mnemonic = "JL"
    elif (word1 & 0xfc00) == 0x3C00: self.mnemonic = "JMP"
    else: assert False, hex(word1)

  def __str__(self):
    return "%s $%d" % (self.mnemonic, (self.Offset * 2) + 2)

#############################################################################

assert len(sys.argv) > 2
elfname = sys.argv[1]
funcname = sys.argv[2]

l = loader.Loader(elfname)
elf = l.get_ELF()
symbol = elf.find_symbol_by_name(funcname)
assert symbol

stream = io.BytesIO(elf.extract_bytes(symbol))
f = InstructionFactory()
instrs = []
while True:
  word1 = read_word(stream)
  if word1 == None: break
  instrs.append(f.create(word1))
  instrs[-1].parse(stream, word1)
  print(instrs[-1])
