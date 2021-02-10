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
      int tidx = (idx + 1) & tmask;
      int fidx = idx       & fmask;
      idx = tidx | fidx;
      int tv = src      & tmask;
      int fv = mst[idx] & fmask;
      mst[idx] = tv | fv;

      /* mst[++idx] = tgt; */
      tidx = (idx + 1) & tmask;
      fidx = idx       & fmask;
      idx = tidx | fidx;
      tv = tgt      & tmask;
      fv = mst[idx] & fmask;
      mst[idx] = tv | fv;

      /* par[src] = tgt; */
      tv = tgt      & tmask;
      fv = par[src] & fmask;
      par[src] = tv | fv;
    }
  }

  mst[0] = idx / 2 + 1;
}
