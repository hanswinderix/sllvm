#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "mulmod8.h"

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

  sancus_enable(&mulmod8);

  ATTACK(mulmod8_enter, 512, 5);
  ATTACK(mulmod8_enter, 5, 512);
  ATTACK(mulmod8_enter, 127, 5);

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
