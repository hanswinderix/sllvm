#ifndef INDIRECT_H
#define INDIRECT_H

extern struct SancusModule indirect;

__attribute__((eentry)) int indirect_enter(__attribute((secret)) int a, int b);

#endif
