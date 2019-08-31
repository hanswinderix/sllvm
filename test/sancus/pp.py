import sys
import re

import matplotlib.pyplot as plt

header_pos = 0
header = """
0 time
1 tb_openMSP430.cur_tsc[63:0]
2 tb_openMSP430.msp_debug_0.inst_pc[15:0]
3 tb_openMSP430.dut.execution_unit_0.exec_sm
4 tb_openMSP430.msp_debug_0.inst_full[255:0]

==========================================================================================
0                x    x 0                                                                x
""".strip().split('\n')

# Parse command line
assert len(sys.argv) > 1, "Argument expected"
exename       = sys.argv[1]
sim_output    = "%s.sim" % exename
vcdcat_output = "%s.vcdcat" % exename

# Parse vcdcat output
signals = []
with open(vcdcat_output) as f:
  prev_inst_pc = 0
  for line in f:
    try:
      # Parse the next line of the vcdcat output
      t, cur_tsc, inst_pc, exec_sm, inst_full = line.split()
      inst_pc = int(inst_pc, 16)
      if (prev_inst_pc != inst_pc):
        cur_tsc   = int(cur_tsc, 16)
        exec_sm   = int(exec_sm, 16)
        inst_full = inst_full[:-1] # Drop trailing 'L'
        inst_full = [inst_full[i:i + 2] for i in range(0, len(inst_full), 2)]
        inst_full = "".join([chr(int(x, 16)) for x in inst_full])
        inst_full = inst_full.replace('\0', '').strip()
        signals.append([cur_tsc, inst_pc, exec_sm, inst_full])
        prev_inst_pc = inst_pc
    except:
      # Assert this is the expected line of the header and just skip it
      # TODO: This should also include header line number 5
      assert line.strip() == header[header_pos].strip(), \
                                               (line, header[header_pos])
      header_pos = header_pos + 1

# Compute instruction latencies
z = list(zip(signals[1:], signals[:-1]))
for idx in range(len(signals)-1):
  l, r = z[idx]
  signals[idx][0] = l[0] - r[0]

# Parse simulation standard output
attacks = []
with open(sim_output) as f:
  attacks = re.findall(r'attack: (.*)?', f.read())

#############################################################################

for epoch in signals:
  print(epoch)
print(attacks)

"""
with open(fname) as f:
  l = re.findall(r'latency: (\d+)', f.read())
  l = [int(x) for x in l if int(x) <= 6]
  plt.plot(l)
  plt.ylabel('some numbers')
  plt.show()
"""
