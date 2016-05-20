#ifndef _DHCP_H_
#define _DHCP_H_

#include "ip.h"
#include "udp.h"

// Length in bytes
#define CLI_HW_ADDR_L 200
// Length in bytes
#define DHCP_DATA_STRUCT_L (8 * 8) + (CLI_HW_ADDR_L )
#define DHCP_DATA_L 0xFFFF - IP_HEADER_L - UDP_HEADER_L - DHCP_DATA_STRUCT_L
#define DHCP_SEND_PORT 68
#define DHCP_DEST_PORT 67
#define DHCP_B_FLAG 1
#define DHCP_MAGIC_COOKIE 0x63825363

#define DHCP_MESSAGE_TYPE 53
#define DHCP_MESSAGET_DISCOVER 1
#define DHCP_MESSAGET_OFFER 2
#define DHCP_MESSAGET_REQUEST 3
#define DHCP_MESSAGET_DECLINE 4
#define DHCP_MESSAGET_ACK 5
#define DHCP_MESSAGET_RELEASE 6

typedef struct __attribute__((__packed__)) dhcp_data {
	byte_t op;
	byte_t htype;
	byte_t hlen;
	byte_t hops;
	uint32_t xid;
	uint16_t secs;
	uint16_t flags;
	ipv4_addr_t cli_addr;
	ipv4_addr_t your_ip;
	ipv4_addr_t serv_ip;
	ipv4_addr_t gate_ip;
	byte_t cli_hw_addr[ CLI_HW_ADDR_L ];
	uint32_t cookie;
} dhcp_data_t;

typedef struct __attribute__((__packed__)) dhcp_packet {
	ipv4_header_t ip_header;
	udp_header_t udp_header;
	dhcp_data_t dhcp_data;
	byte_t data[ DHCP_DATA_L ];
} dhcp_packet_t;

/*
** dhcp_discover
**
** Writes a DHCP packet used to request an IP from DHCP servers
**
** Arguments:
**     dhcp_packet_t *dest - Location to write the packet
** Returns:
**     dhcp_packet_t * ----- Location of the written packet
*/
dhcp_packet_t *dhcp_discover( dhcp_packet_t *dest );

/*
** dhcp_discover_copy
**
** Returns a pointer to a pre made DHCP packet that may be used for requesting
** an IP address from a DHCP server
**
** Returns:
**     dhcp_packet_t * ----- Location of the request packet
*/
packet_t *dhcp_discover_copy( void );

/*
** dhcp_data_hton
**
** Changes the order of the fields of the DHCP packet for network transmission
**
** Arguments:
**     dhcp_data_t *p - location of the DHCP data object to convert
*/
void dhcp_data_hton( dhcp_data_t *p );

/*
** dhcp_data_hton
**
** Changes the order of the fields of the DHCP packet for host parsing
**
** Arguments:
**     dhcp_data_t *p - location of the DHCP data object to convert
*/
void dhcp_data_ntoh( dhcp_data_t *p );

#endif
