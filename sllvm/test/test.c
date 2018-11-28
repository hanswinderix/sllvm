#include <sancus_support/sm_io.h>

int main(void)
{
  msp430_io_init();

  pr_info("Hello World");

  EXIT();
}
