#include "modexp2.h"

#define MOD 7

int modexp2_enter(int y, __attribute__((secret)) int k)
{
  int r = 1;

  for (int i = 0; i < (sizeof(int) * 8); i++)
  {
    /* if ((k % 2) == 1) */
    {
      /* Compute true and false masks */
      int condition = ((k % 2) == 1);
      int tmask = -condition;
      int fmask = ~tmask;

      /* r = (r * y) % MOD; */
      int tr = ((r * y) % MOD) & tmask;
      int fr = r               & fmask;
      r = tr | fr;
    }

    y = (y * y) % MOD;
    k >>= 1;
  }

  return r % MOD;
}
