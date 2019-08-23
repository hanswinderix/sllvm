#ifndef SM_KEYPAD_H
#define SM_KEYPAD_H

#define PIN_LEN     4
#define NB_KEYS     16

extern struct SancusModule keypad_mmio;
extern struct SancusModule keypad;

__attribute__((eentry)) int keypad_init(void);
__attribute__((eentry)) int keypad_poll(void);

#endif
