#ifndef FORK_H
#define FORK_H

extern struct SancusModule fork;

__attribute__((eentry)) int fork_enter(__attribute__((secret)) int a, int b);

#endif
