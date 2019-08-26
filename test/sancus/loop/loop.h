#ifndef LOOP_H
#define LOOP_H

extern struct SancusModule loop;

__attribute__((eentry)) int loop_enter(__attribute((secret)) int a, int b);

#endif
