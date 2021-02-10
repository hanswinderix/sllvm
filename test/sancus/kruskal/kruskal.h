#ifndef KRUSKAL_H
#define KRUSKAL_H

extern struct SancusModule kruskal;

__attribute__((eentry)) void
kruskal_enter(__attribute__((secret)) int g[], int mst[], int par[], int len);

#endif
