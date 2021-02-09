#ifndef SHAREVALUE_H
#define SHAREVALUE_H

extern struct SancusModule sharevalue;

__attribute__((eentry))
int sharevalue_enter(__attribute__((secret)) int ids[], int qty[], int len);

#endif
