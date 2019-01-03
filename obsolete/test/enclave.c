#include <sancus_support/sm_io.h>

DECLARE_SM(enclave, 0x1234);

static int secret;

int SM_ENTRY(enclave) enclave_public_get_secret(void)
{
  pr_info("Incrementing...");

  return ++secret;
}
