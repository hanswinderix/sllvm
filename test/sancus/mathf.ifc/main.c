#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "mathf.h"

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

  sancus_enable(&mathf);

  ATTACK(mathf_enter, 0, 3);
  ATTACK(mathf_enter, 1, 3);
  ATTACK(mathf_enter, 54.567, 14346.67);

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
