#ifndef DIAMOND_H
#define DIAMOND_H

extern struct SancusModule diamond;

__attribute__((eentry)) int diamond_enter(__attribute((secret)) int a, int b);

#endif
