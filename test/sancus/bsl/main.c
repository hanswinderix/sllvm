#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"
#include "bsl.h"

asm(".section __interrupt_vector_10,\"ax\",@progbits \n\t"
    ".word timerA_isr_entry2                         \n\t");

void attack(void)
{
  __ss_print_latency();
}

int main(void)
{
  char data[BSL_PASSWORD_LENGTH];

  msp430_io_init();
  __ss_init();

  sancus_enable(&bsl);

  __ss_mount();
  BSL430_unlock_BSL(data);

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
