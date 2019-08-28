#ifndef IFCOMPOUND_H
#define IFCOMPOUND_H

extern struct SancusModule ifcompound;

__attribute__((eentry))
int ifcompound_enter(__attribute((secret)) int a, int b, int c);

#endif
