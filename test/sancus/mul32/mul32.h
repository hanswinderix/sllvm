#ifndef MUL32_H
#define MUL32_H

typedef unsigned long UInt32;

extern struct SancusModule mul32;

__attribute__((eentry)) UInt32 mul32_enter(__attribute__((secret)) UInt32 a,
                                           __attribute__((secret)) UInt32 b);

#endif
