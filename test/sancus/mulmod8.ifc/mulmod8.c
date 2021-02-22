#include "mulmod8.h"

#define mulMask    0x00FF
#define mulModulus 0x0101

int mulmod8_enter(__attribute__((secret)) int a, int b)
{
  int p;

  a &= mulMask;
  b &= mulMask;

  /* if (a == 0) */
  {
    /* Compute true and false masks */
    int condition = (a == 0);
#if 0 /* Avoid excessive amount of memory operations */
    int tmask = -condition;
    int fmask = ~tmask;
#endif

    /* a = mulModulus - b; */
    a = ((mulModulus - b) & -condition) | (a & ~(-condition));
  }
  /* else if (b == 0) */
  {
    /* Compute true and false masks */
    int condition = (b == 0);
#if 0 /* Avoid excessive amount of memory operations */
    int tmask = -condition;
    int fmask = ~tmask;
#endif

    /*  a = mulModulus - a; */
    a = ((mulModulus - a) & -condition) | (a & ~(-condition));
  }
  /* else */
  {
    /* Compute true and false masks */
    int condition = (a != 0) & (b != 0);
    int tmask = -condition;
    int fmask = ~tmask;

    /* p = a * b; */
    p = ((a * b) & tmask) | (p & fmask);

    /* b = p & mulMask; */
    b = ((p & mulMask) & tmask) | (b & fmask);

    /* a = p >> 8; */
    a = ((p >> 8) & tmask) | (a & fmask);

    /* a = b - a + (b < a ? 1 : 0); */
#if 0
    int condition2 = (b < a); /* Clang generates a branch instruction */
#else
    int condition2 = ((b - a) & 0x8000);
#endif
#if 0 /* Avoid excessive amount of memory operations */
    int tmask2 = -condition;
    int fmask2 = ~tmask2;
#endif
    a = ((b - a) & tmask) | (a & fmask);
    a += (1 & -condition2) | (0 & ~(-condition2));
  }

  return a & mulMask;
}
