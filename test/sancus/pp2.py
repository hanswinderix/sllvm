import sys
import os
import re

import loader

def get_cycles(fname):
  with open(fname, 'r') as f:
    m = re.search('^total cycles: (\d*)$', f.read(), re.MULTILINE)
    assert m, '"total cycles" expected'
    return int(m.group(1))
  assert False

def get_size(elfname, pmname):
  l = loader.Loader(elfname)
  pm = l.find_protected_module_by_name(pmname)
  assert pm, pmname
  return pm.get_text_size()

labels         = []
cycleoverheads = []
sizeoverheads  = []
for root, _, files in os.walk("."):
  for fn in files:
    m = re.match('(.*)\.nemdef\.(experiment.*).signals.txt$', fn)
    if m:
      case       = m.group(1)
      experiment = m.group(2)
      labels.append('%s %s' % (case, experiment))

      # Determine space overhead
      velf = '%s/%s.vulnerable' % (root, case)
      helf = '%s/%s.nemdef' % (root, case)
      vsize = get_size(velf, case)
      hsize = get_size(helf, case)
      sizeoverhead = float(hsize)/vsize
      sizeoverheads.append(sizeoverhead)

      # Determine time overhead
      vsignals = '%s.%s.signals.txt' % (velf, experiment)
      hsignals = '%s.%s.signals.txt' % (helf, experiment)
      vcycles = get_cycles(vsignals)
      hcycles = get_cycles(hsignals)
      cycleoverhead = float(hcycles)/vcycles
      cycleoverheads.append(cycleoverhead)

      sys.stdout.write("%s," % case)
      sys.stdout.write("%d," % vsize)
      sys.stdout.write("%d," % hsize)
      sys.stdout.write("%.02f," % round(sizeoverhead, 2))
      sys.stdout.write("%s," % experiment)
      sys.stdout.write("%d," % vcycles)
      sys.stdout.write("%d," % hcycles)
      sys.stdout.write("%.02f," % round(cycleoverhead, 2))
      sys.stdout.write("\n")

import numpy as np
import matplotlib
matplotlib.use('Agg') # Select non-interactive backend
import matplotlib.pyplot as plt

assert len(labels) == len(cycleoverheads)
assert len(labels) == len(sizeoverheads)

x = np.arange(len(labels))
width = 0.30

fig, ax = plt.subplots()
rects1 = \
  ax.bar(x - width/2, cycleoverheads, width, label='Execution time (cycles)')
rects2 = ax.bar(x + width/2, sizeoverheads, width, label='Code size (bytes)')
ax.set_ylabel("Relative overhead")
ax.set_xticklabels(labels, rotation="vertical", fontsize='x-small')
ax.set_xticks(range(len(cycleoverheads)))
ax.set_ylim(bottom=1.0)
#ax.grid(b=True, which='major', color='lightgray', linestyle='-')
#ax.grid(b=True, which='minor', color='lightgray', linestyle=':')
ax.legend()
plt.axhline(y=1.0, color='lightgray', linestyle='-', linewidth='0.1')
plt.axhline(y=2.0, color='lightgray', linestyle='-', linewidth='0.1')
plt.axhline(y=3.0, color='lightgray', linestyle='-', linewidth='0.1')
plt.axhline(y=4.0, color='lightgray', linestyle='-', linewidth='0.1')
plt.axhline(y=5.0, color='lightgray', linestyle='-', linewidth='0.1')
ax.set_aspect(3)
fig.tight_layout()
# TODO: targetdir should be a command line argument
fig.savefig('results/performance.pdf', bbox_inches='tight', pad_inches=0)
