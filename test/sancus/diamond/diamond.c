#include "diamond.h"

int diamond_enter(__attribute__((secret)) int a, int b)
{
  int result;

  /* A series of secret-dependent branches */
  if (a == b)
  {
    result = 0;
  }
  else if (a < b)
  {
    result = 3;
  }
  else
  {
    result = 7;
  }

  /* A secret-independent branch */
  if (b == 10)
  {
    //result *= 2; // times two generates really weird assembly...
    result *= 4;
  }

  return result;
}
