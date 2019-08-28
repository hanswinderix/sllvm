#include "mulhi3.h"

// Verbatim (modulo formatting) from the GCC compiler runtime (libgcc 8.2.0)

int mulhi3_enter (__attribute__((secret)) int x, __attribute__((secret)) int y)
{
  char bit;
  int neg = 0;
  int rv = 0;

  if (y < 0)
  {
    y = - y;
    neg = 1;
  }

  // The original code gives rise to a CFG which is not supported by the
  // prototype compiler due to a violation of one of the well-formedness
  // constraint for loops (required by the simple loop analysis):
  //    - A loop should have a single exit point
#if 0
  for (bit = 0; y && bit < sizeof (y) * 8; bit ++)
  {
#else
  //  The solution would be to transform early exits into plain if-tests
  //  as follows. This transformation is not implemented becasue it does
  //  not change anything wrt the main contributions of this paper.
  for (bit = 0; bit < sizeof (y) * 8; bit ++)
  {
    if (y)
    {
#endif
      if (y & 1)
        rv += x;
      x <<= 1;
      y >>= 1;
    }
  }

  return neg ? - rv : rv;
}
