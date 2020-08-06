#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "keypad.h"

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

  sancus_enable(&keypad);
  sancus_enable(&keypad_mmio);

  /* Ignore initialization */
  ATTACK(keypad_init);

  /* Ignore first call to keypad_poll, which sets up secure linking */
  ATTACK(keypad_poll, 42);

  /* With the current mmio mockup, calling keypad_poll five times in a row
   * covers the whole range of possibe execution times with the current mmio
   * mockup
   */
  ATTACK(keypad_poll, 42);
#if 0 /* For now, only use the keypad benchmark for determining the average
       * performance overhead of the nemdef transformation. Once the compiler
       * bug, related to the handling of two-way branches in shared basic 
       * blocks, has been fixed, enable the underlying calls again.
       */
  ATTACK(keypad_poll, 42);
  ATTACK(keypad_poll, 42);
  ATTACK(keypad_poll, 42);
  ATTACK(keypad_poll, 42);
#endif

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
