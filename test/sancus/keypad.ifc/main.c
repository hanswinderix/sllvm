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
   * covers the full range of execution times.
   */
  ATTACK(keypad_poll, 42);
  ATTACK(keypad_poll, 42);
  ATTACK(keypad_poll, 42);
  ATTACK(keypad_poll, 42);
  ATTACK(keypad_poll, 42);

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
