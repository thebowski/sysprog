#include "ip.h"

ip_addr_t this_ip = {0,0,0,0};

packet_t *packet_cpy( packet_t *dest, packet_t *src ){
	dest->header.version_ihl = src->header.version_ihl;
	dest->header.dscp_ecn = src->header.dscp_ecn;
	dest->header.length = src->header.length;
	dest->header.ident = src->header.ident;
	dest->header.flags_offset = src->header.flags_offset;
	dest->header.ttl = src->header.ttl;
	dest->header.protocol = src->header.protocol;
	dest->header.header_checksum = src->header.checksum;
	dest->header.source_ip = src->header.source_ip;
	dest->header.dest_ip = src->header.dest_ip;

	for ( int i = 0; i < dest->header.length; ++i ){
		dest->packet_data[ i ] = src->packet_data[ i ];
	}
	return dest;
}



ip_header_t *new_ip_header( ip_header_t *d, ipv4_addr_t dest,
			    byte_t protocol, uint16_t len, byte_t ttl = 64,
			    uint16_t flags = FLAG_DF,
			    byte_t dscp = DEFAULT_DSCP, byte_t ecn = DEAULT_ECN
			    ){
	d->version_ihl = ((HEADER_L << 4) & 0xF0) | (IP_VERSION & 0x0F);
	d->dscp_ecn = ((ecn << 6) & 0xC4) | (dscp & 0x3F);
	d->length = htons( len + HEADER_L );
	d->ident = 0;
	d->flags_offset = htons( (flags & 7h) );
	d->ttl = ttl;
	d->protocol = protocol;
	ipv4_addr_cpy( d->dest_ip, &dest );
	ipv4_addr_cpy( d->source_ip, &this_ip );
	d->header_checksum = 0;
	d->header_checksum = ipv4_header_checksum( d );
}

uint16_t ipv4_header_sum( ip_header_t *h ){
	uint32_t sum = 0;
	sum = ((h->version_ihl << 8) | h->dscp_ecn) +
	       d->length + d->ident + d->flags_offset +
		(d->ttl << 8 | d->protocol) +
		(d->header_checksum) +
		(d->source_ip[0] << 8 + d->source_ip[1]) +
		(d->source_ip[2] << 8 + d->source_ip[3]) +
		(d->dest_ip[0] << 8 + d->dest_ip[1]) +
		(d->dest_ip[2] << 8 + d->dest_ip[3]);
	uint32_t carry = (sum & 0xFFFF0000) >> 16;
	while (carry != 0){
		sum = sum & 0x0000FFFF;
		sum = sum + carry;
		carry = (sum & 0xFFFF0000) >> 16;
	}
	return (uint16_t) sum;
}

uint16_t ipv4_header_checksum( ip_header_t *h ){
	uint16_t checksum = ipv4_header_sum( h ) ^ 0xFFFF;
	return checksum;
}

uint16_t ipv4_header_verify( ip_header_t *h ){
	return ipv4_header_sum( h );
}

ip_addr_t *ipv4_addr_cpy( ipv4_addr_t *dest, ipv4_addr_t *src){
	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = src[2];
	dest[3] = src[3];
	return dest;
}
