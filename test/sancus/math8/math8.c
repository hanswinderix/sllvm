#include "math8.h"

/*******************************************************************************
*
*          Name    : 8-bit Math
*          Purpose : Benchmark 8-bit math functions.
*
*******************************************************************************/

static UInt8
add(__attribute__((secret)) UInt8 a, __attribute__((secret)) UInt8 b)
{ 
  return (a + b); 
}

static UInt8
mul(__attribute__((secret)) UInt8 a, __attribute__((secret)) UInt8 b)
{ 
  return (a * b); 
}

static UInt8
div(UInt8 a, UInt8 b)
{ 
  return (a / b); 
}

__attribute__((eentry)) void math8_enter(__attribute__((secret)) UInt8 a,
                                         __attribute__((secret)) UInt8 b)
{
  volatile UInt8 result[4];

  result[0] = a;
  result[1] = b;

  result[2] = add(result[0], result[1]);

  result[1] = mul(result[0], result[2]);

  result[3] = div(result[1], result[2]);

  return;
}
