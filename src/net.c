#include "net.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "ethernet.h"

#define ntohs(x) ((x) << 8 | (x) >> 8)
#define htons(x) ntohs(x)
#define htonl(x) (__builtin_bswap32(x))

static uint32_t IP = htonl(0xc0a80007);  // 192.168.0.7
uint8_t MAC[6]     = {0xde, 0xad, 0xbe, 0xef, 0xbe, 0xef};

static arp_record_t arp_table[ARP_TABLE_SIZE];

bool arp_table_update(uint32_t ip, uint8_t *mac) {
  for (size_t i = 0; i < ARP_TABLE_SIZE; i++) {
    if (!arp_table[i].used) continue;
    if (arp_table[i].ip == ip) {
      memcpy(arp_table[i].mac, mac, ETH_MAC_LENGTH);
      return true;
    }
  }
  return false;
}

void arp_table_add(uint32_t ip, uint8_t *mac) {
  for (size_t i = 0; i < ARP_TABLE_SIZE; i++) {
    if (arp_table[i].used) continue;
    arp_table[i].used = true;
    arp_table[i].ip   = ip;
    memcpy(arp_table[i].mac, mac, ETH_MAC_LENGTH);
    return;
  }
  puts("ARP table full!\n");
}

void arp_reply(eth_hdr_t *eth_hdr) {
  arp_hdr_t *hdr     = (arp_hdr_t *)eth_hdr->payload;
  arp_ipv4_t *packet = (arp_ipv4_t *)hdr->payload;

  hdr->opcode    = ARP_OPCODE_REPLY;
  packet->dst_ip = packet->src_ip;
  packet->src_ip = IP;
  memcpy(packet->dst_mac, packet->src_mac, ETH_MAC_LENGTH);
  eth_get_mac(packet->src_mac);

  hdr->hw_type  = htons(hdr->hw_type);
  hdr->pro_type = htons(hdr->pro_type);
  hdr->opcode   = htons(hdr->opcode);

  uint32_t len = sizeof(eth_hdr_t) + sizeof(arp_hdr_t) + sizeof(arp_ipv4_t);
  eth_send(packet->dst_mac, eth_hdr, len);
}

void arp_process(eth_hdr_t *eth_hdr) {
  arp_hdr_t *hdr = (arp_hdr_t *)eth_hdr->payload;

  hdr->hw_type  = ntohs(hdr->hw_type);
  hdr->pro_type = ntohs(hdr->pro_type);
  hdr->opcode   = ntohs(hdr->opcode);

  if (hdr->hw_type == ARP_HWTYPE_ETHERNET && hdr->pro_type == ETH_ETHERTYPE_IPv4) {
    arp_ipv4_t *packet = (arp_ipv4_t *)hdr->payload;
    bool merge         = arp_table_update(packet->src_ip, packet->src_mac);

    if (packet->dst_ip != IP) { return; }
    if (!merge) arp_table_add(packet->src_ip, packet->src_mac);
    if (hdr->opcode == ARP_OPCODE_REQUEST) { arp_reply(eth_hdr); }
  }
}

void eth_send(uint8_t *mac, eth_hdr_t *hdr, uint32_t len) {
  memcpy(hdr->dst_mac, mac, ETH_MAC_LENGTH);
  eth_get_mac(hdr->src_mac);
  hdr->ether_type = htons(hdr->ether_type);

  eth_transmit_frame((uint8_t *)hdr, len);
}

void eth_process(uint8_t *frame, uint32_t len) {
  eth_hdr_t *hdr  = (eth_hdr_t *)frame;
  hdr->ether_type = ntohs(hdr->ether_type);
  switch (hdr->ether_type) {
    case ETH_ETHERTYPE_ARP:
      arp_process(hdr);
      break;
    default:
      printf("Received unsupported ethertype: %x\n", hdr->ether_type);
  }
}
