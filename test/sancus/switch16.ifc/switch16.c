#include "switch16.h"

UInt16 switch16_case(__attribute__((secret)) UInt16 a)
{
  UInt16 output;
  
  int condition;
  int tmask;
  int fmask;
  int toutput;
  int foutput;

  /* switch (a) */
  {
    /* case 0x0001: output = 0x0001; break; */
    condition = (a == 0x0001);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0001   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0002: output = 0x0002; break; */
    condition = (a == 0x0002);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0002   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0003: output = 0x0003; break; */
    condition = (a == 0x0003);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0003   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0004: output = 0x0004; break; */
    condition = (a == 0x0004);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0004   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0005: output = 0x0005; break; */
    condition = (a == 0x0005);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0005   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0006: output = 0x0006; break; */
    condition = (a == 0x0006);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0006   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0007: output = 0x0007; break; */
    condition = (a == 0x0007);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0007   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0008: output = 0x0008; break; */
    condition = (a == 0x0008);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0008   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0009: output = 0x0009; break; */
    condition = (a == 0x0009);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0009   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x000a: output = 0x000a; break; */
    condition = (a == 0x000a);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x000a   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x000b: output = 0x000b; break; */
    condition = (a == 0x000b);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x000b   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x000c: output = 0x000c; break; */
    condition = (a == 0x000c);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x000c   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x000d: output = 0x000d; break; */
    condition = (a == 0x000d);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x000d   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x000e: output = 0x000e; break; */
    condition = (a == 0x000e);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x000e   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x000f: output = 0x000f; break; */
    condition = (a == 0x000f);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x000f   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0010: output = 0x0010; break; */
    condition = (a == 0x0010);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0010   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;
  }

  return (output);
}
