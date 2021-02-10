#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "sharevalue.h"

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

  sancus_enable(&sharevalue);

  int ids1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int ids2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 42};
  int qty[]  = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  ATTACK(sharevalue_enter, ids1, qty, sizeof(ids1)/sizeof(ids1[0]));
  ATTACK(sharevalue_enter, ids2, qty, sizeof(ids2)/sizeof(ids2[0]));

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
