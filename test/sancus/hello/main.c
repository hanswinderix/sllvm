#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "hello.h"

int main(void)
{
  msp430_io_init();

  printf("Enabling SM hello...\n");
  sancus_enable(&hello);

  hello_enter();

  EXIT();

  return 0;
}
