#ifndef IFTHENLOOPIF_H
#define IFTHENLOOPIF_H

extern struct SancusModule ifthenloopif;

__attribute__((eentry))
int ifthenloopif_enter(__attribute((secret)) int a, int b);

#endif
