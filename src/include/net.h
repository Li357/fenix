#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "ethernet.h"

#define ETH_CRC_LENGTH     (4)
#define ETH_MAC_LENGTH     (6)
#define ETH_PAYLOAD_OFFSET (14)
#define ETH_ETHERTYPE_IPv4 (0x0800)
#define ETH_ETHERTYPE_ARP  (0x0806)

// Reference: https://www.iana.org/assignments/arp-parameters/arp-parameters.xhtml
#define ARP_HWTYPE_ETHERNET    (1)
#define ARP_OPCODE_REQUEST     (1)
#define ARP_OPCODE_REPLY       (2)
#define ARP_TABLE_SIZE         (64)

#define IP_PROTO_ICMP          (1)

#define ICMP_TYPE_ECHO_REPLY   (0)
#define ICMP_TYPE_ECHO_REQUEST (8)

typedef struct {
  uint8_t dst_mac[6];
  uint8_t src_mac[6];
  uint16_t ether_type;
  uint8_t payload[];
} __attribute__((packed)) eth_hdr_t;

typedef struct {
  uint16_t hw_type;
  uint16_t pro_type;
  uint8_t hw_len;
  uint8_t pro_len;
  uint16_t opcode;
  uint8_t payload[];
} __attribute__((packed)) arp_hdr_t;

typedef struct {
  uint8_t src_mac[6];
  uint32_t src_ip;
  uint8_t dst_mac[6];
  uint32_t dst_ip;
} __attribute__((packed)) arp_ipv4_t;

typedef struct {
  uint32_t ip;
  uint8_t mac[6];
  bool used;
} arp_record_t;

typedef struct {
  // Combined version and header length because of endian weirdness
  uint8_t v_hdr_len;
  // uint8_t version : 4;
  // uint8_t hdr_len : 4;  // In 32-bit words
  uint8_t tos;
  uint16_t len;
  uint16_t id;
  uint8_t flags   : 3;
  uint16_t offset : 13;
  uint8_t ttl;
  uint8_t pro;
  uint16_t csum;
  uint32_t src_addr;
  uint32_t dst_addr;
  uint8_t payload[];
} __attribute__((packed)) ipv4_hdr_t;

typedef struct {
  uint8_t type;
  uint8_t code;
  uint16_t csum;
  uint8_t payload[];
} __attribute__((packed)) icmpv4_t;

typedef struct {
  uint16_t id;
  uint16_t seq;
  uint8_t payload[];
} __attribute__((packed)) icmpv4_echo_t;

void eth_send(uint8_t *mac, eth_hdr_t *frame, uint32_t len);
void eth_process(uint8_t *frame, uint32_t len);
