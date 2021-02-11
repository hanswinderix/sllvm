#ifndef MODEXP2_H
#define MODEXP2_H

extern struct SancusModule modexp2;

__attribute__((eentry))
int modexp2_enter(int y, __attribute__((secret)) int k);

#endif
