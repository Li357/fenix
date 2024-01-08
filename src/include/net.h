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
#define IP_PROTO_TCP           (6)

#define ICMP_TYPE_ECHO_REPLY   (0)
#define ICMP_TYPE_ECHO_REQUEST (8)

#define TCP_MAX_PORTS          (128)
#define TCP_CONTROL_MASK       (0x3FUL)
#define TCP_CONTROL_FIN        (1UL << 0)
#define TCP_CONTROL_SYN        (1UL << 1)
#define TCP_CONTROL_RST        (1UL << 2)
#define TCP_CONTROL_PSH        (1UL << 3)
#define TCP_CONTROL_ACK        (1UL << 4)
#define TCP_CONTROL_URG        (1UL << 5)

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
  uint8_t ver_hdr_len;  // version and header length
  uint8_t tos;
  uint16_t len;
  uint16_t id;
  uint16_t flags_offset;
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

typedef struct {
  uint16_t src_port;
  uint16_t dst_port;
  uint32_t seq;
  uint32_t ack;
  uint16_t hdr_len_control;
  uint16_t window_size;
  uint16_t csum;
  uint16_t urgent_ptr;
  uint8_t payload[];
} __attribute__((packed)) tcp_hdr_t;

typedef enum {
  TCP_LISTEN,
  TCP_SYN_RECEIVED,
  TCP_ESTABLISHED,
} tcp_state_t;

typedef struct {
  uint32_t snd_next;
  uint32_t snd_unack;
  uint32_t rcv_next;
  tcp_state_t state;
} tcp_record_t;

void eth_send(uint8_t *mac, eth_hdr_t *frame, uint32_t len);
void eth_process(uint8_t *frame, uint32_t len);
