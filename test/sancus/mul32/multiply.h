#ifndef MULTIPLY_H
#define MULTIPLY_H

typedef unsigned long UInt32;

extern struct SancusModule multiply;

__attribute__((eentry)) UInt32 multiply_enter(__attribute__((secret)) UInt32 a,
                                              __attribute__((secret)) UInt32 b);

#endif
