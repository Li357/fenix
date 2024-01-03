#pragma once

#include <stddef.h>
#include "stm32f7.h"

#define ETH_MACCR_FESSHIFT       (14)
#define ETH_MACCR_DMSHIFT        (11)
#define ETH_MACCR_TE             (1UL << 3)
#define ETH_MACCR_RE             (1UL << 2)
#define ETH_MACMIIAR_PASHIFT     (11)
#define ETH_MACMIIAR_MRSHIFT     (6)
#define ETH_MACMIIAR_MW          (1UL << 1)
#define ETH_MACMIIAR_MB          (1UL << 0)
#define ETH_MACFFR_PM            (1UL << 0)
#define ETH_DMABMR_SR            (1UL << 0)
#define ETH_DMABMR_AAB           (1UL << 25)
#define ETH_DMABMR_PBLSHIFT      (8)
#define ETH_DMABMR_PBL           (0x1FUL << ETH_DMABMR_PBLSHIFT)
#define ETH_DMAOMR_ST            (1UL << 13)
#define ETH_DMAOMR_SR            (1UL << 1)
#define ETH_DMAOMR_RSF           (1UL << 25)
#define ETH_DMAOMR_TSF           (1UL << 21)
#define ETH_DMAIER_NISE          (1UL << 16)
#define ETH_DMASR_RS             (1UL << 6)
#define ETH_DMASR_RBUS           (1UL << 7)

#define ETH_PHY_ADDR_DEFAULT     (0)
#define ETH_PHY_BCR              (0)
#define ETH_PHY_BCR_ANEN         (1UL << 12)
#define ETH_PHY_BCR_SR           (1UL << 0)
#define ETH_PHY_ANAR             (4)
#define ETH_PHY_ANAR_100BASETXFD (1UL << 8)
#define ETH_PHY_ANAR_100BASETX   (1UL << 7)
#define ETH_PHY_ANAR_10BASETXFD  (1UL << 6)
#define ETH_PHY_ANAR_10BASETX    (1UL << 5)
#define ETH_PHY_ANAR_SELDEFAULT  (0x0001)
#define ETH_PHY_ISR              (29)
#define ETH_PHY_IMR              (30)
#define ETH_PHY_IMR_ANC          (1UL << 6)
#define ETH_PHY_SCSR             (31)
#define ETH_PHY_SCSR_SPEEDSHIFT  (2)
#define ETH_PHY_SCSR_SPEED       (0x7UL << ETH_PHY_SCSR_SPEEDSHIFT)

#define ETH_PHY_10BASETX         (0b001)
#define ETH_PHY_10BASETXFD       (0b101)
#define ETH_PHY_100BASETX        (0b010)
#define ETH_PHY_100BASETXFD      (0b110)

typedef volatile struct eth_reg_t {
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
  uint32_t MACPMTCSR;
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
  uint32_t _RESERVED2[40];

  // MMC register stuff here
  uint32_t _MMC[384]; /* offset 0x100 */

  // IEEE 1588 time stamp stuff here but I'm too lazy to map that all out
  uint32_t _PTPT[576]; /* offset 0x700 */

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

typedef volatile struct {
  uint32_t DES0;
  uint32_t DES1;
  uint32_t DES2;
  uint32_t DES3;
} eth_des_t;

// Arbitrary number of buffers to fit 802.3 packets of at most 1536 bytes
#define ETH_TX_BUFFER_NUM  (4)
#define ETH_TX_BUFFER_SIZE (1536)
#define ETH_RX_BUFFER_NUM  (4)
#define ETH_RX_BUFFER_SIZE (1536)

#define ETH_TDES0_IC       (1UL << 30)
#define ETH_TDES0_TCH      (1UL << 20)
#define ETH_RDES0_LS       (1UL << 9)
#define ETH_RDES0_FS       (1UL << 8)
#define ETH_RDES0_RCH      (1UL << 14)
#define ETH_RDES0_ES       (1UL << 15)
#define ETH_RDES0_FLSHIFT  (16)
#define ETH_RDES0_FL       (0x3FFF << ETH_RDES0_FLSHIFT)
#define ETH_RDES0_OWN      (1UL << 31)
#define ETH_RDES1_RBS1     (0x1FFFUL)

typedef enum {
  ETH_ERR_NONE,
  ETH_ERR_EMPTY,
  ETH_ERR_INVALID_FRAME,
} eth_err_rx_t;

typedef void (*eth_receive_frame_cb_t)(uint8_t *frame, size_t len);

extern volatile int _eth_received_frame;
void eth_init();
eth_err_rx_t eth_receive_frame();
void eth_on_receive_frame(eth_receive_frame_cb_t cb);
