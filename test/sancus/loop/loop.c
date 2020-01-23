#include "loop.h"

static int v = 42;
static int w = 777;

int loop_enter(__attribute__((secret)) int a, int b)
{
  int result = 1;
  int i = 0;

  while (i < b)
  {
    int t = (a & i);

    result *= (t == 0x01) ? v++ : w++;
    i++;
  }

  return result;
}
