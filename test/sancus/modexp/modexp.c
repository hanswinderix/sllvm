#include "modexp.h"

int modexp_enter(int base, __attribute__((secret)) int exp, int mod)
{
  int result = 0;

  if (mod != 1)
  {
    result = 1;
    base = base % mod;

    while (exp > 0)
    {
      if ((exp & 0x01) == 0x01)
      {
        result = (result * base) % mod;
      }

      exp >>= 1;
      base = (base * base) % mod;
    }
  }

  return result;
}
