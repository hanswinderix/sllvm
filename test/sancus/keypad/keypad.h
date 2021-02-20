#ifndef KEYPAD_H
#define KEYPAD_H

#define PIN_LEN     4
#define NB_KEYS     16

extern struct SancusModule keypad;

__attribute__((eentry)) int keypad_init(void);

/* keypad_poll has a dummy parameter merely to trigger the Nemesis SLLVM
 * countermeasure for the function.
 */
__attribute__((eentry)) int keypad_poll(__attribute__((secret)) int dummy);


#endif
