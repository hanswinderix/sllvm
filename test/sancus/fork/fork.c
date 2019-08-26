#include "fork.h"

int fork_enter(__attribute__((secret)) int a, int b)
{
  int result = 3;

  if (a < b)
  {
    result = a + 2;
  }

  return result;
}
