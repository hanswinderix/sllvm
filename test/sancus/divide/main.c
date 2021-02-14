#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "divide.h"

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

  sancus_enable(&divide);

  ATTACK(divide_enter, 0, 1);
  ATTACK(divide_enter, 1, 1);
  ATTACK(divide_enter, 2, 2);
  ATTACK(divide_enter, 8, 6);
  ATTACK(divide_enter, 16, 8);
  ATTACK(divide_enter, 127, 7);

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
