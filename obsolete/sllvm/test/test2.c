#include <sancus_support/sm_io.h>

int SM_ENTRY(enclave) enclave_public_get_secret(void);

int main(void)
{
  int v;

  msp430_io_init();

  printf("Init done.\n");

  v = enclave_public_get_secret();
  printf("v=%d\n", v);
  printf("v=%d\n", enclave_public_get_secret());
  printf("v=%d\n", enclave_public_get_secret());

  EXIT();
}
