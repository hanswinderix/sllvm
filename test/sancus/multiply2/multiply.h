#ifndef MULTIPLY_H
#define MULTIPLY_H

extern struct SancusModule multiply;

__attribute__((eentry)) int multiply_enter(__attribute__((secret)) int a,
                                           __attribute__((secret)) int b);

#endif
