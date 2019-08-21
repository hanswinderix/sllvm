#include "bsl.h"

#define MASS_ERASE_DELAY 0x8000

#define LOCKED   0x00
#define UNLOCKED 0xA5A4
static unsigned LockedStatus = LOCKED;

static unsigned BSL430_massErase(void)
{
  return SUCCESSFUL_OPERATION;
}

/*
 * The C code below is based on the invulnerable password comparison routine of
 *  the latest TI BSL v9.
 */

/*******************************************************************************
*Function:    BSL430_unlock_BSL
*Description: Causes the BSL to compare the data buffer against the BSL password
*             BSL state will be UNLOCKED if successful
*Parameters:
*             char* data           A pointer to an array containing the password
*Returns:
*             SUCCESSFUL_OPERATION  All data placed into data array successfully
*             BSL_PASSWORD_ERROR    Correct Password was not given
*******************************************************************************/
char BSL430_unlock_BSL(__attribute__((secret)) char * data)
{
  int    i;
  int    retValue = 0;
  char * ivt      = (char *) INTERRUPT_VECTOR_START;

  for (i=0; i <= (INTERRUPT_VECTOR_END - INTERRUPT_VECTOR_START); i++, ivt++)
  {
#ifdef V2_2_09
  /* To prevent timing side-channels, versions from v2.2.09 onwards uses a
   * password comparison loop based on XOR, as recommended in Goodspeed2008 to
   * prevent timing side-channels. */
    retValue |=  *ivt ^ data[i];
#else
   /* This vulnerable version is based on published asm code from v2.12. */
    if (*ivt != data[i])
    {
      retValue |= 0x40;
    }
#endif
  }

  if (retValue == 0)
  {
    LockedStatus = UNLOCKED;

    return SUCCESSFUL_OPERATION;
  }
  else
  {
    (void) BSL430_massErase();

    return BSL_PASSWORD_ERROR;
  }
}
