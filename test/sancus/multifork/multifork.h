#ifndef MULTIFORK_H
#define MULTIFORK_H

extern struct SancusModule multifork;

__attribute__((eentry)) int multifork_enter(__attribute((secret)) int a);

#endif
