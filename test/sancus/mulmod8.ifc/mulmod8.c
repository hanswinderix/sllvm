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
    int tmask = -condition;
    int fmask = ~tmask;

    /* a = mulModulus - b; */
    int ta = (mulModulus - b) & tmask;
    int fa = a                & fmask;
    a = ta | fa;
  }
  /* else if (b == 0) */
  {
    /* Compute true and false masks */
    int condition = (b == 0);
    int tmask = -condition;
    int fmask = ~tmask;

    /*  a = mulModulus - a; */
    int ta = (mulModulus - a) & tmask;
    int fa = a                & fmask;
    a = ta | fa;
  }
  /* else */
  {
    /* Compute true and false masks */
    int condition = (a != 0) & (b != 0);
    int tmask = -condition;
    int fmask = ~tmask;

    /* p = a * b; */
    int tp = (a * b) & tmask;
    int fp = p       & fmask;
    int p = tp | fp;

    /* b = p & mulMask; */
    int tb = (p & mulMask) & tmask;
    int fb = b             & fmask;
    int b = tb | fb;

    /* a = p >> 8; */
    int ta = (p >> 8) & tmask;
    int fa = a        & fmask;
    a = ta | fa;

    /* a = b - a + (b < a ? 1 : 0); */
    int t;
    {
#if 0
      int condition = (b < a); /* Clang generates a branch instruction */
#else
      int condition = ((b - a) & 0x8000);
#endif
      int tmask = -condition;
      int fmask = ~tmask;
      int tt = 1 & tmask;
      int ft = 0 & fmask;
      t = tt | ft;
    }
    ta = (b - a) & tmask;
    fa = a       & fmask;
    a = ta | fa;
    a += t;
  }

  return a & mulMask;
}
