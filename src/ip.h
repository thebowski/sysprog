#ifndef _IP_H_
#define _IP_H_

// Header for Internet Protocol
#define IP_HEADER_LENGTH
#define MAX_DATA_LENGTH 0xFFFF - ( IP_HEADER_LENGTH );
#define IP_VERSION 4
#define HEADER_L 5
#define DEFAULT_DSCP 0
#define DEFAULT_ECN 0
#define DEFAULT_TTL 64
#define FLAG_DF 2
#define FLAG_MF 4
#define IMCP_PROTOCOL 1
#define TCP_PROTOCOL 6
#define UDP_PROTOCOL 17

typedef ipv4_addr_t byte_t[4];

typedef struct ip_headers {
	uint8_t version_ihl;
	uint8_t dscp_ecn;
	uint16_t length;
	uint16_t ident;
	uint16_t flags_offset;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t header_checksum;
	ipv4_addr_t source_ip;
	ipv4_addr_t dest_ip;
} ip_header_t;

typedef struct packet {
	ip_header_t header;
	byte_t packet_data[ MAX_DATA_LENGTH ];
} packet_t;

extern ip_addr_t this_ip;

/*
** packet_cpy
**
** Copies a packet from the source to the destination
**
** Arguments:
**     packet_t *dest - pointer to the destination of the copied packet
**     packet_t *src -- Pointer to the packet to copy
** Returns:
**     packet_t * ----- Pointer to the copied packet
*/
packet_t *packet_cpy( packet_t *dest, packet_t *src );

/*
** new_ip_header
**
** This function sets all of the fields of an IP header to the values specified
** when calling the function
**
** Arguments:
**     ip_header_t *d --- Location of header to modify
**     ipv4_addr_t dest - IP address to send the packet to
**     byte_t protocol -- Protocol that the packet data uses (RFC 790)
**     uint16_t len ----- Length of the packet data.  Size of the header is
**                        accounted for automatically
**     byte_t ttl ------- Packet time-to-live (default = 64)
**     uint16_t flags --- Packet flags (default = don't fragment)
**     byte_t dscp ------ Differentiated Services Code Point (default = 0)
**     byte_t ecn ------- Explicit Congestion Notification (default = 0)
** Returns:
**     ip_header_t * ---- Pointer to the header with the fields set
*/
ip_header_t *new_ip_header( ip_header_t *d, ipv4_addr_t dest,
			    byte_t protocol, uint16_t len,
			    byte_t ttl = DEFAULT_TTL, uint16_t flags = FLAG_DF,
			    byte_t dscp = DEFAULT_DSCP, byte_t ecn = DEAULT_ECN
			    );

/*
** ipv4_addr_cpy
**
** Copies the IP address pointed to by src to dest
**
** Arguments:
**     ipv4_addr_t *dest - Location to copy the ipv4 address to
**     ipv4_addr_t *src -- location from which to copy the ipv4 address
** Returns:
**     ipv4_addr_t * ----- Pointer to the destination of the copy
*/
ipv4_addr_t *ipv4_addr_cpy( ipv4_addr_t *dest, ipv4_addr_t *src);

/*
** ipv4_header_checksum
**
** Calculates the checksum of a given ipv4 header
**
** Arguments:
**     ipv4_header_t *h - Header of which to calculate the checksum
** Returns:
**     uint16_t --------- Calculated checksum of the header
*/
uint16_t ipv4_header_checksum( ipv4_header_t *h );

/*
** ipv4_header_verify
**
** Checks the header against its checksum to verify that it is correct
**
** Arguments:
**     ipv4_header_t *h - pointer to the header to verify
**     uint16_t --------- Return value of verification.  0 if valid, non 0
**                        otherwise
*/
uint16_t ipv4_header_verify( ipv4_header_t *h );

#endif
