#include "enclave3.h"

/* External data declaration (should not be marked private by transformation) */
extern int extern_data;

/* The eentry attribute is attached on the prototype only */
void enclave3_public_get_secret(void)
{
  extern_data = 0;
}

#if 0
EENTRY int a; // Clang emits a warning here
#endif

