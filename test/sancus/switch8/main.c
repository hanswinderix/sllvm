#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "switch8.h"

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

  sancus_enable(&switch8);

  ATTACK(switch8_case, 0x01);
  ATTACK(switch8_case, 0x02);
  ATTACK(switch8_case, 0x03);
  ATTACK(switch8_case, 0x04);
  ATTACK(switch8_case, 0x05);
  ATTACK(switch8_case, 0x06);
  ATTACK(switch8_case, 0x07);
  ATTACK(switch8_case, 0x08);
  ATTACK(switch8_case, 0x09);
  ATTACK(switch8_case, 0x0a);
  ATTACK(switch8_case, 0x0b);
  ATTACK(switch8_case, 0x0c);
  ATTACK(switch8_case, 0x0d);
  ATTACK(switch8_case, 0x0e);
  ATTACK(switch8_case, 0x0f);
  ATTACK(switch8_case, 0x10);

  ATTACK(switch8_case, 0x00); /* None of the cases (there is no default case) */

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
