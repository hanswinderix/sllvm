#include <stdio.h>
#include <msp430.h>
#include <sancus_support/sm_io.h>

extern int sllvm_text_section_start;
extern int sllvm_data_section_start;

__attribute__((eentry))
int enclave4_public_get_secret(int p1);

unsigned sllvm_protect(void);

char * HANS = "Hans\n";

__attribute__((section(".init9"),aligned(2)))
int main(void)
{
  unsigned id=0;
  int result;

  msp430_io_init();

  id = sllvm_protect();
  printf("id= %u\n", id);
  result = enclave4_public_get_secret(7);
  printf("result= %d\n", result);
  //printf("text=%d\n", sllvm_text_section_start);
  //printf("data=%d\n", sllvm_data_section_start);
  printf("done\n");

  return 0;
}
