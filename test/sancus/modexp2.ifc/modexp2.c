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
#if 0 /* Avoid excessive amount of memory operations */
      int tmask = -condition;
      int fmask = ~tmask;
#endif

      /* r = (r * y) % MOD; */
      r = (((r * y) % MOD) & -condition) | (r & ~(-condition));
    }

    y = (y * y) % MOD;
    k >>= 1;
  }

  return r % MOD;
}
