#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "multifork.h"

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

  sancus_enable(&multifork);

  __ss_mount();
  multifork_enter(12);

  __ss_mount();
  multifork_enter(13);

  __ss_mount();
  multifork_enter(69);

  __ss_mount();
  multifork_enter(10);

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
