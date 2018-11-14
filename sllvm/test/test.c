#include "sancus/sm_support.h"

int SM_ENTRY(enclave) enclave_public_get_secret(void);

__attribute__((section(".init9"),aligned(2)))
int main(void)
{
  /* Call to enclave code from unprotected code */
  return enclave_public_get_secret();
}
