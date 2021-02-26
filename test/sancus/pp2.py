import sys
import os
import re
import functools

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

# Generate CSV
fname = '%s/performance.csv' % result_dir
with open(fname, 'w') as f:

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
        vsize2, hsize2, l = ifc

        _, vcycles2, hcycles2 = find_experiment(l, experiment)

        f.write("%.02f," % round(float(hsize2)/vsize, 2))
        f.write("%.02f," % round(float(hsize2)/hsize, 2))
        f.write("%.02f," % round(float(hcycles2)/vcycles, 2))
        f.write("%.02f," % round(float(hcycles2)/hcycles, 2))

      f.write("\n")

# Generate LaTeX performance tables for EuroS&P paper

l1 = (
  'call',
  'diamond',
  'fork',
  'ifcompound',
  'ifthenloop',
  'ifthenloopif',
  'ifthenlooploop',
  'ifthenlooplooptail',
  'indirect',
  'loop',
  'multifork',
  'triangle'
)

l2 = (
  'bsl',
  'keypad',
  'kruskal',
  'modexp2',
  'mulhi3',
  'mulmod8',
  'sharevalue',
  'switch8',
  'switch16',
  'twofish'
)

fname1 = '%s/table1.tex' % result_dir
fname2 = '%s/table2.tex' % result_dir
with open(fname1, 'w') as f1:
  with open(fname2, 'w') as f2:
    
    def get_optimum(experiments):
      result = (None, 0, 0) # (expname, vcycles, hcycles)
      for expname, vcycles, hcycles in experiments:
        if vcycles > result[1]:
          result = (expname, vcycles, hcycles)
      return result

    def mean(l):
      return functools.reduce(lambda x,y: x*y, l) ** (1.0 / len(l))

    # Self-made benchmarks
    f = f1
    lhsizes = []
    lhcycles = []
    loptcycles = []
    for benchmark in sorted([x for x in results.keys() if x in l1]):

      vsize, hsize, experiments = results[benchmark]

      f.write(benchmark)
      f.write(" & ")
      f.write("%d" % vsize)
      f.write(" & ")

      komma = ''
      for expname, vcycles, hcycles, in experiments:
        f.write(komma)
        komma = ', '
        f.write("%d" % vcycles)

      f.write(" & ")
      v = float(hsize)/vsize
      lhsizes.append(v)
      f.write("%.02fx" % round(v, 2))
      f.write(" & ")

      komma = ''
      for expname, vcycles, hcycles in experiments:
        f.write(komma)
        komma = ', '
        v = float(hcycles)/vcycles
        lhcycles.append(v)
        f.write("%.02fx" % round(v, 2))

      expname, vcycles, hcycles = get_optimum(experiments)

      f.write(" & ")
      v = float(hcycles)/vcycles
      loptcycles.append(v)
      f.write("%.02fx" % round(v, 2))

      f.write(r"\\")
      f.write("\n")

    f.write("\hline\n")
    f.write("\hline\n")
    f.write("\\textbf{Geometric mean}")
    f.write(" & ")
    f.write(" & ")
    f.write(" & ")
    f.write("\\textbf{%.02fx}" % mean(lhsizes))
    f.write(" & ")
    f.write("\\textbf{%.02fx}" % mean(lhcycles))
    f.write(" & ")
    f.write("\\textbf{%.02fx}" % mean(loptcycles))
    f.write(r"\\")
    f.write("\n")

    # External benchmarks
    f = f2
    lhsizes2 = []
    lhcycles2 = []
    lifcsizes = []
    lifccycles = []
    for benchmark in sorted([x for x in results.keys() if x in l2]):

      vsize, hsize, experiments = results[benchmark]

      expname, vcycles, hcycles = get_optimum(experiments)

      f.write(benchmark)
      f.write(" & ")
      f.write("%d" % vsize)
      f.write(" & ")
      f.write("%d" % vcycles)
      f.write(" & ")

      v = float(hsize)/vsize
      lhsizes2.append(v)
      f.write("%.02fx" % round(v, 2))
      f.write(" & ")
      v = float(hcycles)/vcycles
      lhcycles2.append(v)
      f.write("%.02fx" % round(v, 2))
      f.write(" & ")

      ifc = find_benchmark_ifc(benchmark) 
      assert ifc, benchmark
      _, ifcsize, l   = ifc
      _, _, ifccycles = find_experiment(l, expname)

      v = float(ifcsize)/vsize
      lifcsizes.append(v)
      f.write("%.02fx" % round(v, 2))
      f.write(" & ")
      v = float(ifccycles)/vcycles
      lifccycles.append(v)
      f.write("%.02fx" % round(v, 2))

      f.write(r"\\")
      f.write("\n")

    f.write("\hline\n")
    f.write("\hline\n")
    f.write("\\textbf{Geometric mean}")
    f.write(" & ")
    f.write(" & ")
    f.write(" & ")
    f.write("\\textbf{%.02fx}" % mean(lhsizes2))
    f.write(" & ")
    f.write("\\textbf{%.02fx}" % mean(lhcycles2))
    f.write(" & ")
    f.write("\\textbf{%.02fx}" % mean(lifcsizes))
    f.write(" & ")
    f.write("\\textbf{%.02fx}" % mean(lifccycles))
    f.write(r"\\")
    f.write("\n")

    # Output geometric means of all benchmarks
    lhsizes.extend(lhsizes2)
    loptcycles.extend(lhcycles2)
    print("Geometric mean overhead size  : %.02f" % mean(lhsizes))
    print("Geometric mean overhead cycles: %.02f" % mean(loptcycles))
