#ifndef MULHI3_H
#define MULHI3_H

extern struct SancusModule mulhi3;

__attribute__((eentry))
int mulhi3_enter (__attribute__((secret)) int x, __attribute__((secret)) int y);

#endif
