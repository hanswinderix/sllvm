#include "sharevalue.h"

#define SPECIAL_SHARE 42

static int lookupVal(int id)
{
  return 7;
}

int sharevalue_enter(__attribute__((secret)) int ids[], int qty[], int len)
{
  int shareVal = 0;
  int i = 0;

  while (i < len) 
  {
    int id = ids[i];
    int val = lookupVal(id) * qty[i];

    /* if (id == SPECIAL_SHARE) */
    {
      /* Compute true and false masks */
      int condition = (id == SPECIAL_SHARE);
      int tmask = -condition;
      int fmask = ~tmask;

      /* shareVal = shareVal + val; */
      shareVal = ((shareVal + val) & tmask) | (shareVal & fmask);
    }

    i++;
  }

  return shareVal;
}
