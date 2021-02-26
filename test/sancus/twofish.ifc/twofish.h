#ifndef twofish_H
#define twofish_H

#include <stdint.h>

extern struct SancusModule twofish;

__attribute__((eentry)) void
twofish_key_schedule(__attribute__((secret)) const uint8_t key[], int length);

#endif
