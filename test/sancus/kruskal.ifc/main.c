#include <stdio.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "kruskal.h"

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

  sancus_enable(&kruskal);

  int g[] = {1, 2, 1, 3, 1, 4, 4, 5, 6, 7};
  int mst[sizeof(g)];
  int par[sizeof(g)];

  ATTACK(kruskal_enter, g, mst, par, sizeof(g)/sizeof(g[0]));

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
