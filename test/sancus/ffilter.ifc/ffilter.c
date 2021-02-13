#include "ffilter.h"
 
/*******************************************************************************
*
*          Name    : FIR Filter
*          Purpose : Benchmark a FIR filter. The input values for the filter
*          is an array of 51 16-bit values. The order of the filter
*          17.
*
*******************************************************************************/

#ifdef MSP430
#include "msp430x13x.h"
#endif

#include <math.h>

#define FIR_LENGTH 17

const float COEFF[FIR_LENGTH] = 
{
  -0.000091552734,
   0.000305175781,
   0.004608154297,
   0.003356933594,
  -0.025939941406,
  -0.044006347656,
   0.063079833984,
   0.290313720703,
   0.416748046875,
   0.290313720703,
   0.063079833984,
  -0.044006347656,
  -0.025939941406,
   0.003356933594,
   0.004608154297,
   0.000305175781,
  -0.000091552734
};


/* The following array simulates input A/D converted values */

const unsigned int INPUT[] = 
{
    0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00, 0x2000,
    0x2400, 0x2000, 0x1C00, 0x1800, 0x1400, 0x1000, 0x0C00, 0x0800,
    0x0400, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00,
    0x2000, 0x2400, 0x2000, 0x1C00, 0x1800, 0x1400, 0x1000, 0x0C00,
    0x0800, 0x0400, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800,
    0x1C00, 0x2000, 0x2400, 0x2000, 0x1C00, 0x1800, 0x1400, 0x1000,
    0x0C00, 0x0800, 0x0400
};

__attribute__((eentry)) void ffilter_enter(void)
{
  int i, y; /* Loop counters */

  volatile float OUTPUT[36];

#ifdef MSP430
  WDTCTL = WDTPW + WDTHOLD; /* Stop watchdog timer */
#endif

  for (y = 0; y < 36; y++) 
  {
    for (i = 0; i < FIR_LENGTH / 2; i++) 
    {
      OUTPUT[y] = COEFF[i] * (INPUT[y + 16 - i] + INPUT[y + i]);
    };

    OUTPUT[y] = OUTPUT[y] + (INPUT[y + 16 - i] * COEFF[i]);
  }

  return;
}
