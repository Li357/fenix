#include "stm32f7.h"

#define ETH_DMABMR_SR (1UL << 0)

typedef volatile struct {
  uint32_t MACCR;
  uint32_t MACFFR;
  uint32_t MACHTHR;
  uint32_t MACHTLR;
  uint32_t MACMIIAR;
  uint32_t MACMIIDR;
  uint32_t MACFCR;
  uint32_t MACVLANTR;
  uint32_t _RESERVED0[2];
  uint32_t MACRWUFFR;
  uint32_t _RESERVED1;
  uint32_t MACDBGR;
  uint32_t MACSR;
  uint32_t MACIMR;
  uint32_t MACA0HR;
  uint32_t MACA0LR;
  uint32_t MACA1HR;
  uint32_t MACA1LR;
  uint32_t MACA2HR;
  uint32_t MACA2LR;
  uint32_t MACA3HR;
  uint32_t MACA3LR;
  uint32_t _RESERVED2[16];

  uint32_t MMCCR; /* offset 0x100 */
  uint32_t MMCRIR;
  uint32_t MMCTIR;
  uint32_t MMCRIMR;
  uint32_t MMCTIMR;
  uint32_t _RESERVED3[14];
  uint32_t MMCTGFSCCR; /* offset 0x14C */
  uint32_t MMCTGFMSCCR;
  uint32_t _RESERVED4[5];
  uint32_t MMCTGFCR; /* offset 0x168 */
  uint32_t _RESERVED5[10];
  uint32_t MMCRFCECR; /* offset 0x194 */
  uint32_t MMCRFAECR;
  uint32_t _RESERVED6[12];
  uint32_t MMCRGUFCR; /* offset 0x1C4 */
  uint32_t _RESERVED7[335];

  // IEEE 1588 time stamp stuff here but I'm too lazy to map that all out
  uint32_t _PTPT[192]; /* offset 0x700 */

  uint32_t DMABMR; /* offset 0x1000 */
  uint32_t DMATPDR;
  uint32_t DMARPDR;
  uint32_t DMARDLAR;
  uint32_t DMATDLAR;
  uint32_t DMASR;
  uint32_t DMAOMR;
  uint32_t DMAIER;
  uint32_t DMAMFBOCR;
  uint32_t DMARSWTR;
  uint32_t DMACHTDR;
  uint32_t DMACHRDR;
  uint32_t DMACHTBAR;
  uint32_t DMACHRBAR;
} eth_reg_t;

#define ETH ((eth_reg_t *)ETHERNET_BASE)

void eth_init();
