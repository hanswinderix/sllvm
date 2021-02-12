#include "switch8.h"

UInt8 switch8_case(__attribute__((secret)) UInt8 a)
{
  UInt8 output;
  
  int condition;
  int tmask;
  int fmask;
  int toutput;
  int foutput;

  /* switch (a) */
  {
    /* case 0x01: output = 0x01; break; */
    condition = (a == 0x01);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x01   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x02: output = 0x02; break; */
    condition = (a == 0x02);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x02   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x03: output = 0x03; break; */
    condition = (a == 0x03);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x03   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x04: output = 0x04; break; */
    condition = (a == 0x04);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x04   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x05: output = 0x05; break; */
    condition = (a == 0x05);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x05   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x06: output = 0x06; break; */
    condition = (a == 0x06);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x06   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x07: output = 0x07; break; */
    condition = (a == 0x07);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x07   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x08: output = 0x08; break; */
    condition = (a == 0x08);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x08   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x09: output = 0x09; break; */
    condition = (a == 0x09);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x09   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0a: output = 0x0a; break; */
    condition = (a == 0x0a);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0a   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0b: output = 0x0b; break; */
    condition = (a == 0x0b);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0b   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0c: output = 0x0c; break; */
    condition = (a == 0x0c);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0c   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0d: output = 0x0d; break; */
    condition = (a == 0x0d);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0d   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0e: output = 0x0e; break; */
    condition = (a == 0x0e);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0e   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x0f: output = 0x0f; break; */
    condition = (a == 0x0f);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x0f   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;

    /* case 0x10: output = 0x10; break; */
    condition = (a == 0x10);
    tmask = -condition;
    fmask = ~tmask;
    toutput = 0x10   & tmask;
    foutput = output & fmask;
    output = toutput | foutput;
  }

  return (output);
}
