#ifndef _UDP_H_
#define _UDP_H_

#include "ip.h"
#include "types.h"
#include "netconv.h"

#define UDP_HEADER_L 8
#define UDP_MAX_DATA_LEN 0xFFFF - IP_HEADER_L - UDP_HEADER_L

typedef struct udp_header {
	uint16_t source_port;
	uint16_t dest_port;
	uint16_t length;
	uint16_t checksum;
} udp_header_t;

typedef struct udp_packet {
	ipv4_header_t ip_header;
	udp_header_t udp_header;
	byte_t data[ UDP_MAX_DATA_LEN ];
} udp_packet_t;

/*
** new_udp_header
**
** Writes a new UDP header to the destination
**
** Arguments:
**     udp_header_t *h ------ A pointer to the location to write the header
**     uint16_t source_port - The port this packet is being sent from
**     uint16_t dest_port --- The destination port of the packet
**     uint16_t length ------ Length of the data attached to the packet
** Returns:
**     udp_header_t * ------- A pointer to the written heaer
*/
udp_header_t *new_udp_header( udp_header_t *h, uint16_t source_port,
			      uint16_t dest_port, uint16_t length );

/*
** udp_checksum
**
** Computes the checksum of the UDP header and data
**
** Arguments:
**     udp_packet_t *p --- A pointer to the packet of which to compute the
**                         checksum
**     ipv4_addr_t *dest - Destination IP address of the packet.  Necessary for
**                         the pseudo-IP header used for checksum calculation
**     ipv4_addr_t *src -- Source IP of the packet.  Same purpose as above
** Returns:
**     uint16_t ---------- The checksum of the packet
*/
uint16_t udp_checksum( udp_packet_t *p, ipv4_addr_t *dest,
		       ipv4_addr_t *src );

/*
** udp_verify
**
** Verifies that the UDP packet's checksum matches the data
**
** Arguments:
**     udp_packet_t *p - Pointer to the packet to verify
** Returns:
**     uint16_t -------- 0 if valid, non-0 if invalid
*/
uint16_t udp_verify( udp_packet_t *p );

/*
** udp_header_ntoh
**
** Converts the byte order of the fields of the UDP header for network
** transmission
**
** Arguments:
**     udp_header_t * - Pointer to UDP header to convert
*/
void udp_header_ntoh( udp_header_t *h );

/*
** udp_header_ntoh
**
** Converts the byte order of the fields of the UDP header for host parsing
**
** Arguments:
**     udp_header_t * - Pointer to UDP header to convert
*/
void udp_header_hton( udp_header_t *h );

#endif
