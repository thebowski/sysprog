/*
** Author:    Alexander Bobowski
** Class:     Systems Programming
** Professor: Prof. Carithers
** Date:      5/19/2016
*/

#ifndef _ICPM_H_
#define _ICMP_H_

#include "ip.h"
#include "types.h"

#define ICMP_HEADER_L 8
#define ICMP_MAX_DATA_LEN 0xFFFF - IP_HEADER_L - ICMP_HEADER_L

#define ICMP_T_ECHO_REPLY 0
#define ICMP_T_DEST_UNREACHABLE 3
#define ICMP_T_REDIR_MSG 5
#define ICMP_T_ECHO_REQUEST 8
#define ICMP_T_ROUTER_AD 9
#define ICMP_T_ROUTER_SOL 10
#define ICMP_T_TIME_EXCEEDED 11
#define ICMP_T_BAD_IP_HEADER 12
#define ICMP_T_TIMESTAMP 13
#define ICMP_T_TIMESTAMP_REPLY 14

typedef struct icmp_header {
	byte_t type;
	byte_t code;
	uint16_t checksum;
	uint32_t rest_of_header;
} icmp_header_t;

typedef struct icmp_packet {
	ipv4_header_t ip_header;
	icmp_header_t icmp_header;
	byte_t data[ ICMP_MAX_DATA_LEN ];
} imcp_packet_t;

#endif
