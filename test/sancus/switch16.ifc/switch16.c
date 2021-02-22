#include "switch16.h"

UInt16 switch16_case(__attribute__((secret)) UInt16 a)
{
  UInt16 output;
  
  int condition;
#if 0  /* Avoid excessive amount of memory operations */
  int tmask;
  int fmask;
#endif

  /* switch (a) */
  {
    /* case 0x0001: output = 0x0001; break; */
    condition = (a == 0x0001);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0001 & -condition) | (output & ~(-condition));

    /* case 0x0002: output = 0x0002; break; */
    condition = (a == 0x0002);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0002 & -condition) | (output & ~(-condition));

    /* case 0x0003: output = 0x0003; break; */
    condition = (a == 0x0003);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0003 & -condition) | (output & ~(-condition));

    /* case 0x0004: output = 0x0004; break; */
    condition = (a == 0x0004);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0004 & -condition) | (output & ~(-condition));

    /* case 0x0005: output = 0x0005; break; */
    condition = (a == 0x0005);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0005 & -condition) | (output & ~(-condition));

    /* case 0x0006: output = 0x0006; break; */
    condition = (a == 0x0006);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0006 & -condition) | (output & ~(-condition));

    /* case 0x0007: output = 0x0007; break; */
    condition = (a == 0x0007);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0007 & -condition) | (output & ~(-condition));

    /* case 0x0008: output = 0x0008; break; */
    condition = (a == 0x0008);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0008 & -condition) | (output & ~(-condition));

    /* case 0x0009: output = 0x0009; break; */
    condition = (a == 0x0009);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0009 & -condition) | (output & ~(-condition));

    /* case 0x000a: output = 0x000a; break; */
    condition = (a == 0x000a);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x000a & -condition) | (output & ~(-condition));

    /* case 0x000b: output = 0x000b; break; */
    condition = (a == 0x000b);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x000b & -condition) | (output & ~(-condition));

    /* case 0x000c: output = 0x000c; break; */
    condition = (a == 0x000c);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x000c & -condition) | (output & ~(-condition));

    /* case 0x000d: output = 0x000d; break; */
    condition = (a == 0x000d);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x000d & -condition) | (output & ~(-condition));

    /* case 0x000e: output = 0x000e; break; */
    condition = (a == 0x000e);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x000e & -condition) | (output & ~(-condition));

    /* case 0x000f: output = 0x000f; break; */
    condition = (a == 0x000f);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x000f & -condition) | (output & ~(-condition));

    /* case 0x0010: output = 0x0010; break; */
    condition = (a == 0x0010);
#if 0 /* Avoid excessive amount of memory operations */
    tmask = -condition;
    fmask = ~tmask;
#endif
    output = (0x0010 & -condition) | (output & ~(-condition));
  }

  return (output);
}
