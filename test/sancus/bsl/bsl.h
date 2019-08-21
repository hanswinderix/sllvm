#ifndef BSL_H
#define BSL_H

#define INTERRUPT_VECTOR_START 0xFFE0
#define INTERRUPT_VECTOR_END   0xFFFF
#define BSL_PASSWORD_LENGTH    (INTERRUPT_VECTOR_END-INTERRUPT_VECTOR_START+1)


#define SUCCESSFUL_OPERATION 0x00
#define BSL_PASSWORD_ERROR   0x05

extern struct SancusModule bsl;

__attribute__((eentry))
char BSL430_unlock_BSL(__attribute__((secret)) char * data);

#endif
