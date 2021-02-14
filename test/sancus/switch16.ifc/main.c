#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "switch16.h"

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

  sancus_enable(&switch16);

  ATTACK(switch16_case, 0x0001);
  ATTACK(switch16_case, 0x0002);
  ATTACK(switch16_case, 0x0003);
  ATTACK(switch16_case, 0x0004);
  ATTACK(switch16_case, 0x0005);
  ATTACK(switch16_case, 0x0006);
  ATTACK(switch16_case, 0x0007);
  ATTACK(switch16_case, 0x0008);
  ATTACK(switch16_case, 0x0009);
  ATTACK(switch16_case, 0x000a);
  ATTACK(switch16_case, 0x000b);
  ATTACK(switch16_case, 0x000c);
  ATTACK(switch16_case, 0x000d);
  ATTACK(switch16_case, 0x000e);
  ATTACK(switch16_case, 0x000f);
  ATTACK(switch16_case, 0x0010);

  ATTACK(switch16_case, 0x0000); /* None of the cases (there is no default) */

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
