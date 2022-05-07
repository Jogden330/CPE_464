#ifndef TRACEH
#define TRACEH

#include <pcap/pcap.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <netinet/ether.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define BIT_8_SIZE 1
#define BIT_16_SIZE 2
#define BIT_32_SIZE 4
#define IP_SIZE 4
#define MAX_SIZE 6


#define DES_MAC_OFFSET 0
#define DES_MAC_LENGTH 6
#define SOURCE_MAC_OFFSET 6
#define SOURCE_MAC_LENGTH 6
#define TYPE_OFFSET 12
#define TYPE_LENGTH 2
#define ETHERNET_HEADER_LENGTH  14

#define APR_OPCODE_LENGTH 2
#define APR_OPCODE_OFFSET 6
#define APR_SENDER_MAC_LENGTH 6
#define APR_SENDER_MAC_OFFSET 8
#define APR_SENDER_IP_LENGTH 4
#define APR_SENDER_IP_OFFSET 14
#define APR_TARGET_MAC_LENGTH 6
#define APR_TARGET_MAC_OFFSET 18 
#define APR_TARGET_IP_LENGTH 4
#define APR_TARGET_IP_OFFSET 24


#define IP_HEADER_VERSION_OFFSET 0
#define IP_TYPE_OFFSET 1
#define IP_LENGTH_OFFSET 2
#define IP_IDENTITY_OFFSET 4
#define IP_FLAGS_FRAG_OFFSET 6
#define IP_TTL_OFFSET 8
#define IP_PROTOCOL_OFFSET 9
#define IP_CHECKSOME_OFFSET 10
#define IP_SOURCE_IP_OFFSET 12
#define IP_DEST_IP_OFFSET 16

#define ICMP_TYPE_OFFSET 0

#define UDP_SOURCE_OFFSET 0
#define UDP_DES_OFFSET 2

#define TCP_SOURCE_OFFSET 0
#define TCP_DES_OFFSET 2
#define TCP_SEQ_OFFSET 4
#define TCP_ACK_OFFSET 8
#define TCP_FLG_OFFSET 12
#define TCP_WIN_OFFSET 14
#define TCP_CHK_OFFSET 16

#define SUDO_SOURCE_IP_OFFSET 0
#define SUDO_DES_IP_OFFSET 4
#define SUDO_RES_OFFSET 8
#define SUDO_PROT_OFFSET 9
#define SUDO_LENGTH_OFFSET 10 
#define SUDO_HEADER_OFFSET 12


int ethernet_layer(const u_char *frame);
int IP_layer(const u_char *data_gram, int *type, u_char  *sudoheader);
void APR_protocol(const u_char *data);
void ICMP_protocol(const u_char *segment);
void UDP_protocol(const u_char *segment);
void TCP_layer(const u_char *segment, u_char *sudoheader );


#endif
