#include "sancus/sm_support.h"

DECLARE_SM(enclave, 42);

int SM_ENTRY(enclave) enclave_public_get_secret(void)
{
  return 10;
}
