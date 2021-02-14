#ifndef MATH32_H
#define MATH32_H

typedef unsigned long UInt32;

extern struct SancusModule math32;

__attribute__((eentry)) void math32_enter(__attribute__((secret)) UInt32 a,
                                          __attribute__((secret)) UInt32 b);

#endif
