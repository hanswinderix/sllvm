#include "mul32.h"

UInt32 mul32_enter(__attribute__((secret)) UInt32 a,
                   __attribute__((secret)) UInt32 b)
{
  return a * b;
}
