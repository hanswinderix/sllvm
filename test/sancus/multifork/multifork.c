#include "multifork.h"

int multifork_enter(__attribute((secret)) int v)
{
  int result;

  switch (v)
  {
    case 12: result = 123; break;
    case 13: result = 129; break;
    case 69: result = 111; break;

    default: result = 0; break;
  }

  return result;
}
