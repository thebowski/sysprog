#include "ip.h"
#include "netconv.h"

packet_t *packet_cpy( packet_t *dest, packet_t *src ){
	dest->header.version_ihl = src->header.version_ihl;
	dest->header.dscp_ecn = src->header.dscp_ecn;
	dest->header.length = src->header.length;
	dest->header.ident = src->header.ident;
	dest->header.flags_offset = src->header.flags_offset;
	dest->header.ttl = src->header.ttl;
	dest->header.protocol = src->header.protocol;
	dest->header.checksum = src->header.checksum;
	ipv4_addr_cpy( &(dest->header.source_ip), &(src->header.source_ip) );
	ipv4_addr_cpy( &(dest->header.dest_ip), &(dest->header.dest_ip) );

	for ( int i = 0; i < dest->header.length - IP_HEADER_L; ++i ){
		dest->packet_data[ i ] = src->packet_data[ i ];
	}
	return dest;
}



ipv4_header_t *new_ipv4_header( ipv4_header_t *d, ipv4_addr_t dest,
				ipv4_addr_t src, byte_t protocol, uint16_t len,
				byte_t ttl, uint16_t flags, byte_t dscp,
				byte_t ecn ){
	d->version_ihl = ((IP_VERSION << 4) & 0xF0) | (IP_HEADER_L/4 & 0x0F);
	d->dscp_ecn = ((ecn << 6) & 0xC4) | (dscp & 0x3F);
	d->length = len + IP_HEADER_L;
	d->ident = 0;
	d->flags_offset = (flags & 0x7) << 13;
	d->ttl = ttl;
	d->protocol = protocol;
	ipv4_addr_cpy( &(d->dest_ip), &dest );
	ipv4_addr_cpy( &(d->source_ip), &src );
	d->checksum = 0;
	d->checksum = ipv4_header_checksum( d );
	return d;
}

uint16_t ipv4_header_sum( ipv4_header_t *d ){
	uint32_t sum = 0;
	sum = ((d->version_ihl << 8) | d->dscp_ecn) +
	       d->length + d->ident + d->flags_offset +
		(d->ttl << 8 | d->protocol) +
		(d->checksum) +
		((d->source_ip.addr[0] << 8) + d->source_ip.addr[1]) +
		((d->source_ip.addr[2] << 8) + d->source_ip.addr[3]) +
		((d->dest_ip.addr[0] << 8) + d->dest_ip.addr[1]) +
		((d->dest_ip.addr[2] << 8) + d->dest_ip.addr[3]);
	uint32_t carry = (sum & 0xFFFF0000) >> 16;
	while (carry != 0){
		sum = sum & 0x0000FFFF;
		sum = sum + carry;
		carry = (sum & 0xFFFF0000) >> 16;
	}
	return (uint16_t) sum;
}

uint16_t ipv4_header_checksum( ipv4_header_t *h ){
	uint16_t checksum = ipv4_header_sum( h ) ^ 0xFFFF;
	return checksum;
}

uint16_t ipv4_header_verify( ipv4_header_t *h ){
	return ipv4_header_sum( h );
}

ipv4_addr_t *ipv4_addr_cpy( ipv4_addr_t *dest, ipv4_addr_t *src){
	dest->addr[0] = src->addr[0];
	dest->addr[1] = src->addr[1];
	dest->addr[2] = src->addr[2];
	dest->addr[3] = src->addr[3];
	return dest;
}

void ipv4_header_hton( ipv4_header_t *h ){
	h->length = htons( h->length );
	h->ident = htons( h->ident );
	h->flags_offset = htons( h->flags_offset );
	h->checksum = htons( h->checksum );
}

void ipv4_header_ntoh( ipv4_header_t *h ){
	h->length = ntohs( h->length );
	h->ident = ntohs( h->ident );
	h->flags_offset = ntohs( h->flags_offset );
	h->checksum = ntohs( h->checksum );
}
