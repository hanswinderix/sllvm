#ifndef SWITCH8_H
#define SWITCH8_H

typedef unsigned char UInt8;

extern struct SancusModule switch8;

__attribute__((eentry)) UInt8 switch8_case(__attribute__((secret)) UInt8 a);

#endif
