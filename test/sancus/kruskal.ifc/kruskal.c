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
#if 0 /* Avoid excessive amount of memory operations */
      int tmask = -condition;
      int fmask = ~tmask;
#endif

      /* mst[++idx] = src; */
      idx = ((idx + 1) & -condition) | (idx & ~(-condition));
      mst[idx] = (src & -condition) | (mst[idx] & ~(-condition));

      /* mst[++idx] = tgt; */
      idx = ((idx + 1) & -condition) | (idx & ~(-condition));
      mst[idx] = (tgt & -condition) | (mst[idx] & ~(-condition));

      /* par[src] = tgt; */
      par[src] = (tgt & -condition) | (par[src] & ~(-condition));
    }
  }

  mst[0] = idx / 2 + 1;
}
