#include "mulhi3.h"

// Verbatim (modulo formatting) from the GCC compiler runtime (libgcc 8.2.0)

int mulhi3_enter (__attribute__((secret)) int x, __attribute__((secret)) int y)
{
  char bit;
  int neg = 0;
  int rv = 0;

  /* if (y < 0) */
  {
#if 0
    int condition = (y < 0); // Clang generate a branch instruction
#else
    int condition = (y & 0x80000);
#endif
    int tmask = -condition;
    int fmask = ~tmask;

    /* y = - y; */
    y = (-y & tmask) | (y & fmask);

    /* neg = 1; */
    neg = (1 & tmask) | (neg & fmask);
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
    /* if (y) */
    {
      int condition = y;
#if 0 /* Avoid excessive amount of memory operations */
      int tmask = -condition;
      int fmask = ~tmask;
#endif
#endif

      /* if (y & 1) */
      {
        int condition2 = ((y & 1) & -condition);
#if 0 /* Avoid excessive amount of memory operations */
        int tmask = -condition2;
        int fmask = ~tmask;
#endif

        /* rv += x; */
        rv = ((rv + x) & -condition2) | (rv | ~(-condition2));
      }

      /* x <<= 1; */
      x = ((x << 1) & -condition) | (x & ~(-condition));

      /* y >>= 1; */
      y = ((y >> 1) & -condition) | (y & ~(-condition));
    }
  }


  /* if (neg) */
  {
    /* return neg ? - rv : rv; */
    int condition = neg;
    int tmask = -condition;
    int fmask = ~tmask;

    rv = (-rv & tmask) | (rv & fmask);

    return rv;
  }
}
