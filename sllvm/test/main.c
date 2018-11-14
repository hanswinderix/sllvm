#include "sancus/sm_support.h"

int SM_ENTRY(enclave1) enclave1_public_get_secret(void);
int SM_ENTRY(enclave1) enclave1_public_get_secret3(int p1, int p2);

__attribute__((section(".init9"),aligned(2)))
int main(void)
{
  /* Call to enclave code from unprotected code */
  return enclave1_public_get_secret() + enclave1_public_get_secret3(1, 2);
}
