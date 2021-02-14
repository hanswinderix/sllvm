#ifndef DIVIDE_H
#define DIVIDE_H

extern struct SancusModule divide;

__attribute__((eentry)) int divide_enter(__attribute__((secret)) int a,
                                         __attribute__((secret)) int b);

#endif
