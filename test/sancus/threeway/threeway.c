#include <assert.h>

#include "threeway.h"

int i=1;

/* Taken from
 *  "Cryptography: Protocols, Algorithms, and Source Code in C"
 *
 * void mu(word32 *a)
 *  Inverts the order of the bits of a
 */
void threeway_enter(__attribute__((secret)) word32 * a)
{
  int i;
  word32 b[3];

  b[0] = b[1] = b[2] = 0;
  for( i=0 ; i<32 ; i++ )
  {
    b[0] <<= 1;
    b[1] <<= 1;
    b[2] <<= 1;

    if (a[0] & 1) b[2] |= 1;
    if (a[1] & 1) b[1] |= 1;
    if (a[2] & 1) b[0] |= 1;

    a[0] >>= 1;
    a[1] >>= 1;
    a[2] >>= 1;
  }
  a[0] = b[0];
  a[1] = b[1];
  a[2] = b[2];
}
