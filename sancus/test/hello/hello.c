#include "hello.h"
#include "sancus_support/sm_io.h"

static void print_hello(void)
{
  pr_info1("Hello from %d\n", sancus_get_self_id());
}

void hello_enter(void)
{
  print_hello();
}
