#include <stdint.h>
#include <msp430.h>
#include "sancus_support/sm_io.h"
#include "keypad.h"

typedef uint16_t key_state_t;

static int         count = 0;
static key_state_t key_state = 0;
static char        pin[PIN_LEN];
static int         pin_idx;
static int         keymap[NB_KEYS];

static key_state_t read_key_state()
{
  key_state_t result;

  switch (count++)
  {
    case 0: result = 0x00; break;
    case 1: result = 0x01; break;
    case 2: result = 0x01; break;
    case 3: result = 0x00; break;
    case 4: result = 0x02; break;
    case 5: result = 0x00; break;
    case 6: result = 0x04; break;
    case 7: result = 0x00; break;
    case 8: result = 0x08; break;
    case 9: result = 0x00; break;
    case 10: result = 0x10; break;

    default: result = 0x00; break;
  }

  return result;
}

int keypad_init(void)
{
  int result = 1;

  count = 0;
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

  key_state_t new_key_state = read_key_state();

  /* Store down keys in private PIN array. */
  for (int key = 0; key < NB_KEYS; key++)
  {
    int is_pressed  = (new_key_state & key_mask);
    int was_pressed = (key_state & key_mask);
    if ( is_pressed /* INTERRUPT SHOULD ARRIVE HERE */
         && !was_pressed && (pin_idx < PIN_LEN))
    {
      pin[pin_idx++] = keymap[key];
    }
    /* .. OR HERE. */

    key_mask = key_mask << 1;
  }

  key_state = new_key_state;

  return (PIN_LEN - pin_idx);
}

