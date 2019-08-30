#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "ifcompound.h"

asm(".section __interrupt_vector_10,\"ax\",@progbits \n\t"
    ".word timerA_isr_entry2                         \n\t");

void attack(void)
{
  __ss_print_latency();
}

int main(void)
{
  msp430_io_init();
  __ss_init();

  sancus_enable(&ifcompound);

  SS_MOUNT(ifcompound_enter, 1, 1, 2);
  SS_MOUNT(ifcompound_enter, 1, 1, 1);
  SS_MOUNT(ifcompound_enter, 2, 1, 2);

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
