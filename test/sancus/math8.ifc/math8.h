#ifndef MATH8_H
#define MATH8_H

typedef unsigned char UInt8;

extern struct SancusModule math8;

__attribute__((eentry)) void math8_enter(__attribute__((secret)) UInt8 a,
                                         __attribute__((secret)) UInt8 b);

#endif
