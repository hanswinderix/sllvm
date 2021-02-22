#include "switch8.h"

UInt8 switch8_case(__attribute__((secret)) UInt8 a)
{
  UInt8 output;
  
  int condition;
#if 0 /* Avoid excessive amount of memory operations */
  int tmask;
  int fmask;
#endif

  /* switch (a) */
  {
    /* case 0x01: output = 0x01; break; */
    condition = (a == 0x01);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x01 & -condition) | (output & ~(-condition));

    /* case 0x02: output = 0x02; break; */
    condition = (a == 0x02);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x02 & -condition) | (output & ~(-condition));

    /* case 0x03: output = 0x03; break; */
    condition = (a == 0x03);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x03 & -condition) | (output & ~(-condition));

    /* case 0x04: output = 0x04; break; */
    condition = (a == 0x04);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x04 & -condition) | (output & ~(-condition));

    /* case 0x05: output = 0x05; break; */
    condition = (a == 0x05);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x05 & -condition) | (output & ~(-condition));

    /* case 0x06: output = 0x06; break; */
    condition = (a == 0x06);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x06 & -condition) | (output & ~(-condition));

    /* case 0x07: output = 0x07; break; */
    condition = (a == 0x07);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x07 & -condition) | (output & ~(-condition));

    /* case 0x08: output = 0x08; break; */
    condition = (a == 0x08);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x08 & -condition) | (output & ~(-condition));

    /* case 0x09: output = 0x09; break; */
    condition = (a == 0x09);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x09 & -condition) | (output & ~(-condition));

    /* case 0x0a: output = 0x0a; break; */
    condition = (a == 0x0a);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0a & -condition) | (output & ~(-condition));

    /* case 0x0b: output = 0x0b; break; */
    condition = (a == 0x0b);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0b & -condition) | (output & ~(-condition));

    /* case 0x0c: output = 0x0c; break; */
    condition = (a == 0x0c);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0c & -condition) | (output & ~(-condition));

    /* case 0x0d: output = 0x0d; break; */
    condition = (a == 0x0d);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0d & -condition) | (output & ~(-condition));

    /* case 0x0e: output = 0x0e; break; */
    condition = (a == 0x0e);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0e & -condition) | (output & ~(-condition));

    /* case 0x0f: output = 0x0f; break; */
    condition = (a == 0x0f);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0f & -condition) | (output & ~(-condition));

    /* case 0x10: output = 0x10; break; */
    condition = (a == 0x10);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x10 & -condition) | (output & ~(-condition));
  }

  return (output);
}
