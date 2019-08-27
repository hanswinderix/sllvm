#ifndef CALL_H
#define CALL_H

extern struct SancusModule call;

__attribute__((eentry)) int call_enter(__attribute((secret)) int a, int b);

#endif
