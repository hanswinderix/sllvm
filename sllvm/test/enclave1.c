#include "sancus/sm_support.h"

/* External unprotected function */
int abs(int j);

/* External C enclave function */
int SM_ENTRY(enclave2) enclave2_public_get_secret(int p1);

/* External Rust enclave function */
int SM_ENTRY(enclave4) enclave4_public_get_secret(int p1);

DECLARE_SM(enclave1, 42);

/* Local enclave data */
static int SM_DATA(enclave1) enclave1_secret;

/* Local enclave function */
static int SM_FUNC(enclave1) enclave1_private_get_secret(void) 
{
  int result;

  /* External call to C enclave code */
  result = enclave2_public_get_secret(1);

  /* External call to unprotected code */
  result = abs(result);

  return result;
}

/* Public enclave function */
int SM_ENTRY(enclave1) enclave1_public_get_secret(void)
{
  /* Internal call */
  return enclave1_private_get_secret();
}

int SM_ENTRY(enclave1) enclave1_public_get_secret2(int p1)
{
  /* Internal call */
  return 2 * enclave1_private_get_secret();
}

int SM_ENTRY(enclave1) enclave1_public_get_secret3(int p1, int p2)
{
  int result;

  /* External call to Rust enclave code */
  result = enclave4_public_get_secret(p1 + p2);

  return result;
}
