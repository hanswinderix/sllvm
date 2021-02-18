#include <stdint.h>
#include <msp430.h>
#include "sancus_support/sm_io.h"
#include "keypad.h"
#include "keypad_mmio.h"

#define EXPECTED_ID   1

static key_state_t key_state;
static char        pin[PIN_LEN];
static int         pin_idx;
static int         keymap[NB_KEYS];

int keypad_init(void)
{
  int result = 0;

  /* keypad_mmio SM has the ID of this SM hardcoded */
  if (sancus_get_self_id() == EXPECTED_ID)
  {
    /* call and verify keypad_mmio SM */
    keypad_mmio_init();
    pin_idx = 0;
    key_state = 0x0;
    /* avoid an unprotected global constant */
    /* TODO consider an SM_CONST compiler attribute (?) */
    keymap[0]  = '1';
    keymap[1]  = '4';
    keymap[2]  = '7';
    keymap[3]  = '0';
    keymap[4]  = '2';
    keymap[5]  = '5';
    keymap[6]  = '8';
    keymap[7]  = 'F';
    keymap[8]  = '3';
    keymap[9]  = '6';
    keymap[10] = '9';
    keymap[11] = 'E';
    keymap[12] = 'A';
    keymap[13] = 'B';
    keymap[14] = 'C';
    keymap[15] = 'D';

    result = 1;
  }

  return result;
}

/*
 * The start-to-end timing of this function only reveals the number of times the
 * if statement was executed (i.e. the number of keys that were down), which is
 * also explicitly returned. By carefully interrupting the function each for
 * loop iteration, an untrusted ISR can learn the value of the secret PIN code.
 */
/* keypad_poll has a dummy parameter merely to trigger the Nemesis SLLVM
 * countermeasure for the function.
 */
int keypad_poll(__attribute__((secret)) int dummy)
{
  int key_mask = 0x1;

  /* Fetch key state from MMIO driver SM. */
  key_state_t new_key_state = keypad_mmio_read_key_state();

  /* Store down keys in private PIN array. */
  for (int key = 0; key < NB_KEYS; key++)
  {
    int is_pressed  = (new_key_state & key_mask);
    int was_pressed = (key_state & key_mask);

#if 0
    if ( is_pressed /* INTERRUPT SHOULD ARRIVE HERE */
         && !was_pressed && (pin_idx < PIN_LEN))
#endif
    {
      int condition = is_pressed && !was_pressed && (pin_idx < PIN_LEN);
      int tmask = -condition;
      int fmask = ~tmask;

      /* pin[pin_idx++] = keymap[key]; */
      pin_idx = ((pin_idx + 1) & tmask) | (pin_idx & fmask);
      pin[pin_idx] = (keymap[key] & tmask) | (pin[pin_idx] & fmask);
    }
    /* .. OR HERE. */

    key_mask = key_mask << 1;
  }

  key_state = new_key_state;

  return (PIN_LEN - pin_idx);
}
