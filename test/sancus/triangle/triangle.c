#include "triangle.h"

int triangle_enter(__attribute__((secret)) int a, int b)
{
  int result = 3;

  if (a < b)
  {
    result = 7;
  }

  return result;
}
