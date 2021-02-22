import sys
import os
import re

import loader

result_dir = '.'
if len(sys.argv) > 1:
  result_dir = sys.argv[1]

def get_cycles(fname):
  with open(fname, 'r') as f:
    m = re.search('^total cycles: (\d*)$', f.read(), re.MULTILINE)
    assert m, '"total cycles" expected'
    return int(m.group(1))
  assert False

def get_size(elfname, pmname):
  l = loader.Loader(elfname)
  n = pmname.replace(".ifc", "") # Drop if-conversion suffix, if any
  pm = l.find_protected_module_by_name(n)
  assert pm, (pmname, n)
  return pm.get_text_size()

# TODO: Get rid of do_ignore.
#       The first two calls of the benchmark program should not be registered
#       as an attack. Fix this in the benchmark framework, instead of
#       hardcoding it here. (under time pressure now...)
def do_ignore(benchmark, experiment):

  #  - keypad_init call, it does not contain secret-dependent control-flow
  if benchmark in ("keypad", "keypad.ifc") \
      and experiment == "experiment01": return True

  #  - first call to keypad_poll, it sets up secure linking (high latency)
  if benchmark in ("keypad", "keypad.ifc") \
      and experiment == "experiment02": return True

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
results = {}
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

        # Determine the time overheads
        vsignals = '%s.%s.signals.txt' % (velf, experiment)
        hsignals = '%s.%s.signals.txt' % (helf, experiment)
        vcycles = get_cycles(vsignals)
        hcycles = get_cycles(hsignals)

        # Record the results for outputting results in other formats,
        # such as LaTeX
        if benchmark not in results:
          results[benchmark] = (vsize, hsize, [])
        t = (experiment, vcycles, hcycles)
        results[benchmark][2].append(t)

# Generate CSV
fname = '%s/performance.csv' % result_dir
with open(fname, 'w') as f:

  def find_benchmark_ifc(benchmark):
    try:
      return results['%s.ifc' % benchmark]
    except:
      return None

  def find_experiment(l, name):
    for experiment, vcycles, hcycles in l:
      if name == experiment:
        return experiment, vcycles, hcycles
    assert False, "Experiment not found"

  for benchmark in sorted(results.keys()):

    ifc = find_benchmark_ifc(benchmark)

    vsize, hsize, experiments = results[benchmark]
    for experiment, vcycles, hcycles in experiments:

      f.write("%s," % benchmark)
      f.write("%d," % vsize)
      f.write("%d," % hsize)
      f.write("%.02f," % round(float(hsize)/vsize, 2))
      f.write("%s," % experiment)
      f.write("%d," % vcycles)
      f.write("%d," % hcycles)
      f.write("%.02f," % round(float(hcycles)/vcycles, 2))

      if ifc:
        vsize2, _, l = ifc

        _, vcycles2, _ = find_experiment(l, experiment)

        f.write("%.02f," % round(float(vsize2)/vsize, 2))
        f.write("%.02f," % round(float(vsize2)/hsize, 2))
        f.write("%.02f," % round(float(vcycles2)/vcycles, 2))
        f.write("%.02f," % round(float(vcycles2)/hcycles, 2))

      f.write("\n")

# Generate LaTeX performance table for EuroS&P paper
fname = '%s/performance.tex' % result_dir
with open(fname, 'w') as f:

  def writeln(s):
    print(s, file=f)

  f.write(r'''
\documentclass[a4paper]{article}

\usepackage{todonotes}
\usepackage{array}
\newcolumntype{L}[1]{>{\raggedright\let\newline\\\arraybackslash\hspace{0pt}}p{#1}}
\newcolumntype{C}[1]{>{\centering\let\newline\\\arraybackslash\hspace{0pt}}p{#1}}
\newcolumntype{R}[1]{>{\raggedleft\let\newline\\\arraybackslash\hspace{0pt}}p{#1}}

\begin{document}

\begin{table*}[!htb]
\normalsize
\caption{Caption}
\label{tab:benchmarks-overview}
\centering
\scalebox{0.8}{
\begin{tabular}{ L{2.75cm}|C{1.35cm}L{3.4cm}|C{1.35cm}L{4.3cm}C{1.35cm} }
  
\hline
\multicolumn{1}{c|}{\textbf{Benchmark}} & \multicolumn{2}{c|}{\textbf{Vulnerable Baseline}}                 & \multicolumn{3}{c}{\textbf{Overhead of Hardening}}\\
\hline
                                          & \multicolumn{1}{c}{Size}    & \multicolumn{1}{c|}{Execution time} & \multicolumn{1}{c}{Size} & \multicolumn{1}{c}{Execution time} & \multicolumn{1}{c}{Execution time}\\
                                          & \multicolumn{1}{c}{(bytes)} & \multicolumn{1}{c|}{(cycles)}       & \multicolumn{1}{c}{}     & \multicolumn{1}{c}{}               & \multicolumn{1}{c}{(relative to best case)}\\
\hline
\hline



''')

  """
  for benchmark in sorted(results.keys()):
    vsize, hsize, sizeoverhead, experiments = results[benchmark]
    f.write(benchmark)
    f.write(" & ")
    f.write("%d" % vsize)
    f.write(" & ")
    komma = ''
    for name, vcycles, hcycles, acycleoverhead, rcycleoverhead in experiments:
      f.write(komma)
      komma = ', '
      f.write("%d" % vcycles)
    f.write(" & ")
    f.write("%.02fx" % sizeoverhead)
    f.write(" & ")
    komma = ''
    for name, vcycles, hcycles, acycleoverhead, rcycleoverhead in experiments:
      f.write(komma)
      komma = ', '
      f.write("%.02fx" % acycleoverhead)
    f.write(" & ")
    f.write("%dx" % 0) # TODO
    f.write(r"\\")
    f.write("\n")
  """
  
  f.write('''



\hline

\end{tabular}
}
\end{table*}

\end{document}
''')

"""
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
"""
