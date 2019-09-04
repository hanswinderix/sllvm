#include "ifcompound.h"

int ifcompound_enter(__attribute((secret)) int a, int b, int c)
{
  int result;

  /* Secret-dependent branch */
  if ( (a == b) && (b < c) )
  {
    result = 7;
  }
  else
  {
    result = 3;
  }

  /* Secret-independent branch */
  if ( (b ^ c) == 0)
  {
    result *= 3;
  }

  return result;
}

