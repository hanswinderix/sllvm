#include "kruskal.h"

static int find(int x, int par[]) 
{
  if (par[x] != x)
  {
    return find(par[x], par);
  }

  return x;
}

void
kruskal_enter(__attribute__((secret)) int g[], int mst[], int par[], int len)
{
  for (int i=0; i < len; ++i)
  {
    mst[i] = -1;
    par[i] = i;
  }

  int idx = 0;

  for (int i=1; i < len; i += 2)
  {
    int src = find(g[i], par);
    int tgt = find(g[i + 1], par);

    if (src != tgt)
    {
      mst[++idx] = src;
      mst[++idx] = tgt;
      par[src] = tgt;
    }
  }

  mst[0] = idx / 2 + 1;
}
