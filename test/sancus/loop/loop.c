#include "loop.h"

static int v = 42;
static int w = 777;

int loop_enter(__attribute((secret)) int a, int b)
{
  int result = 1;
  int i = 0;

  while (i < b)
  {
    result *= ((a % i) < 3) ? v++ : w++;
    i++;
  }

  return result;
}
