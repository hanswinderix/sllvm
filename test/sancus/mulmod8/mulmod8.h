#ifndef MULMOD8_H
#define MULMOD8_H

extern struct SancusModule mulmod8;

__attribute__((eentry)) int mulmod8_enter(__attribute__((secret)) int a, int b);

#endif
