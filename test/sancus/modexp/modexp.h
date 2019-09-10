#ifndef MODEXP_H
#define MODEXP_H

extern struct SancusModule modexp;

__attribute__((eentry))
int modexp_enter(int base, __attribute__((secret)) int exp, int mod);

#endif
