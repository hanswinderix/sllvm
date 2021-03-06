#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "multiply.h"

asm(".section __interrupt_vector_10,\"ax\",@progbits \n\t"
    ".word timerA_isr_entry2                         \n\t");

void attack(void)
{
  __ss_print_latency();
}

int main(void)
{
  msp430_io_init();

#ifdef SANCUS_STEP_ENABLED
  __ss_init();
#endif

  sancus_enable(&multiply);

#if 1
  ATTACK(multiply_enter, 0, 0);
  ATTACK(multiply_enter, 0, 1);
  ATTACK(multiply_enter, 1, 0);
  ATTACK(multiply_enter, 1, 1);
  ATTACK(multiply_enter, 2, 2);
  ATTACK(multiply_enter, 8, 6);
  ATTACK(multiply_enter, 127, 7);
  ATTACK(multiply_enter, 7, 127);
#else
  printf("%d\n", multiply_enter(0, 0));
  printf("%d\n", multiply_enter(0, 1));
  printf("%d\n", multiply_enter(1, 0));
  printf("%d\n", multiply_enter(1, 1));
  printf("%d\n", multiply_enter(2, 2));
  printf("%d\n", multiply_enter(8, 6));
  printf("%d\n", multiply_enter(127, 7));
  printf("%d\n", multiply_enter(7, 127));
#endif

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
