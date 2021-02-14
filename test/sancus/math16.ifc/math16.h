#ifndef MATH16_H
#define MATH16_H

typedef unsigned short UInt16;

extern struct SancusModule math16;

__attribute__((eentry)) void math16_enter(__attribute__((secret)) UInt16 a,
                                          __attribute__((secret)) UInt16 b);

#endif
