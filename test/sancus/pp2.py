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

for root, _, files in os.walk("."):
  for fn in files:
    m = re.match('(.*)\.nemdef\.(experiment.*).signals.txt$', fn)
    if m:
      case       = m.group(1)
      experiment = m.group(2)

      # Determine space overhead
      velf = '%s/%s.vulnerable' % (root, case)
      helf = '%s/%s.nemdef' % (root, case)
      vsize = get_size(velf, case)
      hsize = get_size(helf, case)
      sizeoverhead = float(hsize)/vsize

      # Determine time overhead
      vsignals = '%s.%s.signals.txt' % (velf, experiment)
      hsignals = '%s.%s.signals.txt' % (helf, experiment)
      vcycles = get_cycles(vsignals)
      hcycles = get_cycles(hsignals)
      cycleoverhead = float(hcycles)/vcycles

      sys.stdout.write("%s," % case)
      sys.stdout.write("%d," % vsize)
      sys.stdout.write("%d," % hsize)
      sys.stdout.write("%.02f," % round(sizeoverhead, 2))
      sys.stdout.write("%s," % experiment)
      sys.stdout.write("%d," % vcycles)
      sys.stdout.write("%d," % hcycles)
      sys.stdout.write("%.02f," % round(cycleoverhead, 2))
      sys.stdout.write("\n")
