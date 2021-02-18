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

    /* if (src != tgt) */
    {
      /* Compute true and false masks */
      int condition = (src != tgt);
      int tmask = -condition;
      int fmask = ~tmask;

      /* mst[++idx] = src; */
      idx = ((idx + 1) & tmask) | (idx & fmask);
      mst[idx] = (src & tmask) | (mst[idx] & fmask);

      /* mst[++idx] = tgt; */
      idx = ((idx + 1) & tmask) | (idx & fmask);
      mst[idx] = (tgt & tmask) | (mst[idx] & fmask);

      /* par[src] = tgt; */
      par[src] = (tgt & tmask) | (par[src] & fmask);
    }
  }

  mst[0] = idx / 2 + 1;
}
