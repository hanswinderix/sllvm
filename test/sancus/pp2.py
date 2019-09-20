import os
import re

def extract_total_cycles(root, fn):
  with open('%s/%s' % (root, fn), 'r') as f:
    m = re.search('^total cycles: (\d*)$', f.read(), re.MULTILINE)
    assert m, '"total cycles" expected'
    return int(m.group(1))
  assert False

for root, dirs, files in os.walk("."):
  for fn in files:
    m = re.match('(.*)\.nemdef\.(experiment.*).signals.txt$', fn)
    if m:
      nemdef     = fn
      case       = m.group(1)
      experiment = m.group(2)
      nemdef     = '%s.nemdef.%s.signals.txt' % (case, experiment)
      vulnerable = '%s.vulnerable.%s.signals.txt' % (case, experiment)
      assert nemdef == fn

      vcycles = extract_total_cycles(root, vulnerable)
      ncycles = extract_total_cycles(root, nemdef)

      print((case, experiment, vcycles, ncycles, round(ncycles/vcycles, 2)))
