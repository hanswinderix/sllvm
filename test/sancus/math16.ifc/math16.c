#include "math16.h"

/*******************************************************************************
*
*          Name    : 8-bit Math
*          Purpose : Benchmark 8-bit math functions.
*
*******************************************************************************/

static UInt16
add(__attribute__((secret)) UInt16 a, __attribute__((secret)) UInt16 b)
{ 
  return (a + b); 
}

static UInt16
mul(__attribute__((secret)) UInt16 a, __attribute__((secret)) UInt16 b)
{ 
  return (a * b); 
}

static UInt16
div(UInt16 a, UInt16 b)
{ 
  return (a / b); 
}

__attribute__((eentry)) void math16_enter(__attribute__((secret)) UInt16 a,
                                          __attribute__((secret)) UInt16 b)
{
  volatile UInt16 result[4];

  result[0] = a;
  result[1] = b;

  result[2] = add(result[0], result[1]);

  result[1] = mul(result[0], result[2]);

  result[3] = div(result[1], result[2]);

  return;
}
