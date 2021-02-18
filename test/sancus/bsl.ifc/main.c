#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "bsl.h"

asm(".section __interrupt_vector_10,\"ax\",@progbits \n\t"
    ".word timerA_isr_entry2                         \n\t");

void attack(void)
{
  __ss_print_latency();
}

int main(void)
{
  char password[BSL_PASSWORD_LENGTH] = "0123456789ABCDEF";

  msp430_io_init();

#ifdef SANCUS_STEP_ENABLED
  __ss_init();
#endif

  sancus_enable(&bsl);

  ATTACK(BSL430_unlock_BSL, password);

  password[2] = 'X';
  ATTACK(BSL430_unlock_BSL, password);

  password[7] = 'X';
  password[8] = 'X';
  ATTACK(BSL430_unlock_BSL, password);

  password[10] = 'X';
  password[11] = 'X';
  ATTACK(BSL430_unlock_BSL, password);

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
