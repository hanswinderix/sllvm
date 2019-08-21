#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "multiply.h"

int main(void)
{
  int a = 4;
  int b = 5;

  msp430_io_init();

  printf("%d x %d = %d\n", a, b, multiply(a, b));

  EXIT();

  return 0;
}
