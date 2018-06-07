#include "sm_support.h"

DECLARE_SM(enclave2, 42);

/* Enclave data definition */
int SM_DATA(enclave2) private_data;

/* Public enclave function with one parameter */
int SM_ENTRY(enclave2) enclave2_public_get_secret(int p1)
{
  private_data = 0;
  return p1;
}

/* Public enclave function with one parameter */
int SM_ENTRY(enclave2) enclave2_public_get_secret2(int p1)
{
  return enclave2_public_get_secret(p1);
}
