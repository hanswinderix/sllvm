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

# TODO: Get rid of do_ignore.
#       The first two calls of the benchmark program should not be registered
#       as an attack. Fix this in the benchmark framework, instead of
#       hardcoding it here. (under time pressure now...)
def do_ignore(benchmark, experiment):

  #  - keypad_init call, it does not contain secret-dependent control-flow
  if benchmark == "keypad" and experiment == "experiment01": return True
  #  - first call to keypad_poll, it sets up secure linking (high latency)
  if benchmark == "keypad" and experiment == "experiment02": return True

  return False

run_times = {}
for root, _, files in os.walk("."):
  for fn in sorted(files):
    m = re.match('(.*)\.nemdef\.(experiment.*).signals.txt$', fn)
    if m:
      benchmark  = m.group(1)
      experiment = m.group(2)
      if not do_ignore(benchmark, experiment):
        velf = '%s/%s.vulnerable' % (root, benchmark)
        vsignals = '%s.%s.signals.txt' % (velf, experiment)

        # Register the running time of this vulnerable experiment. It will be
        # used later for computing the overhead of this experiment relative to
        # the maximum vulnerable running time of all experiments done with this
        # benchmark.
        vcycles = get_cycles(vsignals)
        if not benchmark in run_times:
            run_times[benchmark] = []
        run_times[benchmark].append(get_cycles(vsignals))

# TODO: Factor out the common code of the previous loop.
labels          = []
 # Cycle overheads relative to the corresponding vulnerable experiment
acycleoverheads = []
# Cycle overheads relative to the vulnerable experiment with the highest
# running time
rcycleoverheads = []
sizeoverheads   = []
for root, _, files in os.walk("."):
  for fn in sorted(files):
    m = re.match('(.*)\.nemdef\.(experiment.*).signals.txt$', fn)
    if m:
      benchmark  = m.group(1)
      experiment = m.group(2)
      if not do_ignore(benchmark, experiment):
        labels.append('%s %s' % \
                     (benchmark, str.replace(experiment, "experiment", "exp")))

        velf = '%s/%s.vulnerable' % (root, benchmark)
        helf = '%s/%s.nemdef' % (root, benchmark)

        # Determine the space overhead
        vsize = get_size(velf, benchmark)
        hsize = get_size(helf, benchmark)
        sizeoverhead = float(hsize)/vsize
        sizeoverheads.append(sizeoverhead)

        # Determine the time overheads
        vsignals = '%s.%s.signals.txt' % (velf, experiment)
        hsignals = '%s.%s.signals.txt' % (helf, experiment)
        vcycles = get_cycles(vsignals)
        hcycles = get_cycles(hsignals)
        acycleoverhead = float(hcycles)/vcycles
        acycleoverheads.append(acycleoverhead)
        rcycleoverhead = float(hcycles)/max(run_times[benchmark])
        rcycleoverheads.append(rcycleoverhead)
        
        sys.stdout.write("%s," % benchmark)
        sys.stdout.write("%d," % vsize)
        sys.stdout.write("%d," % hsize)
        sys.stdout.write("%.02f," % round(sizeoverhead, 2))
        sys.stdout.write("%s," % experiment)
        sys.stdout.write("%d," % vcycles)
        sys.stdout.write("%d," % hcycles)
        sys.stdout.write("%.02f," % round(acycleoverhead, 2))
        sys.stdout.write("%.02f," % round(rcycleoverhead, 2))
        sys.stdout.write("\n")

import numpy as np
import matplotlib
matplotlib.use('Agg') # Select non-interactive backend
import matplotlib.pyplot as plt

assert len(labels) == len(acycleoverheads)
assert len(labels) == len(rcycleoverheads)
assert len(labels) == len(sizeoverheads)

x = np.arange(len(labels))
width = 0.30

fig, ax = plt.subplots()
rects1 = \
  ax.bar(x - width/2, acycleoverheads, width, label='Execution time (cycles)')
rects2 = ax.bar(x + width/2, sizeoverheads, width, label='Code size (bytes)')
ax.set_ylabel("Relative overhead")
ax.set_xticklabels(labels, rotation="vertical", fontsize='x-small')
ax.set_xticks(range(len(acycleoverheads)))
ax.set_ylim(bottom=1.0)
#ax.grid(b=True, which='major', color='lightgray', linestyle='-')
#ax.grid(b=True, which='minor', color='lightgray', linestyle=':')
ax.legend()
plt.axhline(y=1.0, color='lightgray', linestyle='-', linewidth='0.1')
plt.axhline(y=2.0, color='lightgray', linestyle='-', linewidth='0.1')
plt.axhline(y=3.0, color='lightgray', linestyle='-', linewidth='0.1')
plt.axhline(y=4.0, color='lightgray', linestyle='-', linewidth='0.1')
plt.axhline(y=5.0, color='lightgray', linestyle='-', linewidth='0.1')
ax.set_aspect(2.7)
fig.tight_layout()
# TODO: targetdir should be a command line argument
fig.savefig('results/performance.pdf', bbox_inches='tight', pad_inches=0)
