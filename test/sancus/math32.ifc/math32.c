#include "math32.h"

/*******************************************************************************
*
*          Name    : 32-bit Math
*          Purpose : Benchmark 32-bit math functions.
*
*******************************************************************************/

static UInt32
add(__attribute__((secret)) UInt32 a, __attribute__((secret)) UInt32 b)
{ 
  return (a + b); 
}

static UInt32
mul(__attribute__((secret)) UInt32 a, __attribute__((secret)) UInt32 b)
{ 
  return (a * b); 
}

static UInt32
div(UInt32 a, UInt32 b)
{ 
  return (a / b); 
}

__attribute__((eentry)) void math32_enter(__attribute__((secret)) UInt32 a,
                                          __attribute__((secret)) UInt32 b)
{
  volatile UInt32 result[4];

  result[0] = a;
  result[1] = b;

  result[2] = add(result[0], result[1]);

  result[1] = mul(result[0], result[2]);

  result[3] = div(result[1], result[2]);

  return;
}
