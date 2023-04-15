import sys
import re

import numpy as np

# Parse command line
assert len(sys.argv) > 1, "Argument expected"
exename       = sys.argv[1]
sim_output    = "%s.sim" % exename
vcdcat_output = "%s.vcdcat" % exename

interactive = False
if len(sys.argv) > 2:
  interactive = True

import matplotlib

# Using matplotlib.use('Agg') selects the non-interactive backend
#  instead of defaulting to Xwindows. This makes sure the scripts runs when
#  when invoked withoug having the DISPLAY env variable set (e.g. Jenkins)
if not interactive:
  matplotlib.use('Agg')

import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator

if interactive:
  import mplcursors # For annotations

header_pos = 0
header = """
0 time
1 TOP.tb_openMSP430.cur_tsc[63:0]
2 TOP.tb_openMSP430.msp_debug_0.inst_pc[15:0]
3 TOP.tb_openMSP430.dut.execution_unit_0.exec_sm
4 TOP.tb_openMSP430.msp_debug_0.inst_full[255:0]

==========================================================================================
0                1    2 3                                                                4
""".strip().split('\n')

# Parse vcdcat output
signals = []
with open(vcdcat_output) as f:
  prev_inst_pc = 0
  for line in f:
    try:
      # Parse the next line of the vcdcat output
      t, cur_tsc, inst_pc, exec_sm, inst_full = line.split()
      inst_pc = int(inst_pc, 16)
      if prev_inst_pc != inst_pc:
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
attack_names = []
sm_text_size = 0
with open(sim_output) as f:
  l = f.read()
  attack_names = re.findall(r'attack: (.*)?', l)

  l = re.findall(r'New SM 1 config: ([0-9a-f]*) ([0-9a-f]*)', l)
  assert len(l) > 0
  if len(l) == 1:
    l = l[0]
    assert len(l) == 2
    l = [int(x, 16) for x in l]
    sm_text_size = l[1] - l[0]
  else:
      # TODO: keypad benchmark instantiates two enclaves
      pass


# Create attack data structures
attacks = []
idx = 0
in_sm = False
for inst_latency, inst_pc, exec_sm, inst_full in signals:
  if exec_sm == 1:
    if not in_sm:
      attacks.append([])
      in_sm = True
    attacks[-1].append([inst_latency, inst_pc, inst_full])
  else:
    in_sm = False

assert len(attacks) == len(attack_names), (len(attacks), len(attack_names))

#############################################################################

fig, axs = plt.subplots(len(attacks), sharex=True)

if len(attacks) == 1:
  axs = np.array([axs])

# Write results
fname = '%s.info' % exename
with open(fname, 'w') as f:
  f.write("sm_text_size=%d\n" % sm_text_size)

for idx in range(len(attacks)):
  total_cycles = 0
  name = attack_names[idx]
  hsize = 16
  vsize = 4

  figi = plt.figure(figsize=(hsize, vsize))
  axi = plt.gca()

  ax = axs[idx]

  # Write signals
  fname = '%s.experiment%02d.signals.txt' % (exename, idx+1)
  with open(fname, 'w') as f:
    f.write("%s\n" % name)
    for inst_latency, inst_pc, inst_full in attacks[idx]:
      total_cycles = total_cycles + inst_latency
      f.write("%d %x %s\n" % (inst_latency, inst_pc, inst_full))
    f.write("total cycles: %d\n" % total_cycles)

  # Write latency trace
  fname = '%s.experiment%02d.latencies.txt' % (exename, idx+1)
  with open(fname, 'w') as f:
    for inst_latency, _, _ in attacks[idx]:
      f.write("%d\n" % inst_latency)

  # Plot latency
  latencies = [signals[0] for signals in attacks[idx]]

  for x in (axi, ax):
    #x.set_title(name)
    x.set_xlabel('instruction number')
    x.set_ylabel('cycles')
    x.set_yticks(np.arange(1, 6, 1))
    #ml = MultipleLocator(1)
    #x.xaxis.set_minor_locator(ml)
    #ml = MultipleLocator(5)
    #x.xaxis.set_major_locator(ml)
    x.grid(b=True, which='major', color='lightgray', linestyle='-')
    x.grid(b=True, which='minor', color='lightgray', linestyle=':')

    # annotations for diamond.vulnerable (see paper)
    """
    if idx == 0:
      x.annotate("",
              xy=(25, 2.3), xycoords='data',
              xytext=(-10, 25), textcoords='offset points',
              arrowprops=dict(color="tab:red", arrowstyle="fancy"))
    if idx == 1:
      x.annotate("",
              xy=(21, 1.3), xycoords='data',
              xytext=(10, 25), textcoords='offset points',
              arrowprops=dict(color="tab:red", arrowstyle="fancy"))
    """

    x.plot(latencies)

  fname = '%s.experiment%02d.pdf' % (exename, idx+1)
  figi.savefig(fname, bbox_inches='tight', pad_inches=0)
  fname = '%s.experiment%02d.svg' % (exename, idx+1)
  figi.savefig(fname, bbox_inches='tight', pad_inches=0)

  if interactive:
    #cursor = mplcursors.cursor(hover=True)
    cursor = mplcursors.cursor(axi)

    @cursor.connect("add")
    def on_add(sel):
      print(sel)
      x, _ = sel.target
      x = int(round(x))
      _, inst_pc, inst_full = attacks[idx][x]
      sel.annotation.set(text="%d: %04X (%s)" % (x, inst_pc, inst_full))

    """
    if idx == (len(attacks)-1):
      figi.show()
    else:
      figi.show()
    """

# Hide x labels and tick labels for top plots and y ticks for right plots.
for ax in axs.flat:
  ax.label_outer()

fname = '%s.pdf' % exename
fig.savefig(fname, bbox_inches='tight', pad_inches =0)
fname = '%s.svg' % exename
fig.savefig(fname, bbox_inches='tight', pad_inches=0)

if interactive:
  plt.show()
