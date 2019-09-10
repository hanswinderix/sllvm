#ifndef THREEWAY_H
#define THREEWAY_H

typedef unsigned long int word32;

extern struct SancusModule threeway;

__attribute__((eentry)) void threeway_enter(__attribute((secret)) word32 * a);

#endif
