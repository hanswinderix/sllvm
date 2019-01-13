#include "hello.h"
#include "sancus_support/sm_io.h"

static int secret = 42;

static void print_hello(void)
{
  pr_info2("Hello from SM %d. My secret is %d \n",
      sancus_get_self_id(),
      secret);
}

void hello_enter(void)
{
  print_hello();
}
