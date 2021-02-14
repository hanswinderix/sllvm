#ifndef SWITCH16_H
#define SWITCH16_H

typedef unsigned char UInt16;

extern struct SancusModule switch16;

__attribute__((eentry)) UInt16 switch16_case(__attribute__((secret)) UInt16 a);

#endif
