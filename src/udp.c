#include "udp.h"

udp_header_t *new_udp_header( udp_header_t *h, uint16_t source_port,
			      uint16_t dest_port, uint16_t length ){
	h->source_port = source_port;
	h->dest_port = dest_port;
	h->length = length + UDP_HEADER_L;
	h->checksum = 0;
	return h;
}

uint16_t udp_checksum( udp_packet_t *p, ipv4_addr_t *dest,
		       ipv4_addr_t *src ){
	uint32_t sum = p->udp_header.source_port +
		p->udp_header.dest_port +
		p->udp_header.length +
		p->udp_header.length +
		((dest->addr[0] << 8) + dest->addr[1]) +
		((dest->addr[2] << 8) + dest->addr[3]) +
		((src->addr[0] << 8) + src->addr[1]) +
		((src->addr[2] << 8) + src->addr[3]) +
		p->udp_header.checksum + UDP_PROTOCOL;

	for (int i = 0; i < p->udp_header.length; ++i){
		if (i % 2 == 0){
			sum = sum + (p->data[i] << 8);
		} else {
			sum = sum + p->data[i];
		}
	}

	uint32_t carry = (sum & 0xFFFF0000) >> 16;
	while (carry != 0){
		sum = sum & 0x0000FFFF;
		sum = sum + carry;
		carry = (sum & 0xFFFF0000) >> 16;
	}

	return (uint16_t)((sum & 0xFFFF) ^ (0xFFFF));
}

uint16_t udp_verify( udp_packet_t *p ){
	return udp_checksum( p, &(p->ip_header.dest_ip),
			     &(p->ip_header.source_ip) );
}
