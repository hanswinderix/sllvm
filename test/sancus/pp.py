import sys
import re

import matplotlib.pyplot as plt

header_pos = 0
header = [
'0 time',
'1 tb_openMSP430.cur_tsc[63:0]',
'2 tb_openMSP430.msp_debug_0.inst_pc[15:0]',
'3 tb_openMSP430.msp_debug_0.inst_full[255:0]',
'',
#'0                1    2                                                                3',
'========================================================================================',
'0                x    x                                                                x',
]

# Parse command line
assert len(sys.argv) > 1, "Argument expected"
exename       = sys.argv[1]
vcdcat_output = "%s.vcdcat" % exename

# Post-process vcdcat output
with open(vcdcat_output) as f:
  prev_inst_pc = 0
  for line in f:
    try:
      # Parse the next line of the vcdcat output
      t, cur_tsc, inst_pc, inst_full = line.split()
      inst_pc = int(inst_pc, 16)
      if (prev_inst_pc != inst_pc):
        cur_tsc   = int(cur_tsc, 16)
        inst_full = inst_full[:-1] # Drop trailing 'L'
        inst_full = [inst_full[i:i + 2] for i in range(0, len(inst_full), 2)]
        inst_full = "".join([chr(int(x, 16)) for x in inst_full])
        print('%d %X "%s"' % (cur_tsc, inst_pc, inst_full))
        prev_inst_pc = inst_pc
    except:
      # Assert this is the expected line of the header and just skip it
      # TODO: This should also include header line number 5
      assert line.strip() == header[header_pos], (line, header[header_pos])
      header_pos = header_pos + 1

"""
with open(fname) as f:
  l = re.findall(r'latency: (\d+)', f.read())
  l = [int(x) for x in l if int(x) <= 6]
  plt.plot(l)
  plt.ylabel('some numbers')
  plt.show()
"""
