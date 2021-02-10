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

  /* 5 vertices, 7 edges */
  int g1[] = {0, 1, 0, 2, 0, 3, 0, 4, 1, 2, 2, 3, 3, 4}; 

  /* 7 vertices, 7 edges */
  int g2[] = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7};
  
  int mst[sizeof(g1)];
  int par[sizeof(g1)];

  ATTACK(kruskal_enter, g1, mst, par, sizeof(g1)/sizeof(g1[0]));
  ATTACK(kruskal_enter, g2, mst, par, sizeof(g2)/sizeof(g2[0]));

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
