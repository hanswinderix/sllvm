#ifndef IFTHENLOOP_H
#define IFTHENLOOP_H

extern struct SancusModule ifthenloop;

__attribute__((eentry))
int ifthenloop_enter(__attribute((secret)) int a, int b);

#endif
