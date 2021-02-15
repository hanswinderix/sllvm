#include "multiply.h"

UInt32 multiply_enter(__attribute__((secret)) UInt32 a,
                      __attribute__((secret)) UInt32 b)
{
  return a * b;
}
