#include "mathf.h"

/*******************************************************************************
*
*          Name    : 32-bit Math
*          Purpose : Benchmark 32-bit math functions.
*
*******************************************************************************/

static float
add(__attribute__((secret)) float a, __attribute__((secret)) float b)
{ 
  return (a + b); 
}

static float
mul(__attribute__((secret)) float a, __attribute__((secret)) float b)
{ 
  return (a * b); 
}

static float
div(float a, float b)
{ 
  return (a / b); 
}

__attribute__((eentry)) void mathf_enter(__attribute__((secret)) float a,
                                         __attribute__((secret)) float b)
{
  volatile float result[4];

  result[0] = a;
  result[1] = b;

  result[2] = add(result[0], result[1]);

  result[1] = mul(result[0], result[2]);

  result[3] = div(result[1], result[2]);

  return;
}
