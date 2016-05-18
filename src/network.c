#include "network.h"

int32_t _request_port( uint16_t port_num, receive_t receive, uint32_t pid ){
	// If the port is currently unallocated, allocated it to the requesting
	// process
	uint32_t owner_pid = ports[ port_num ].owner_pid;
	if ( owner_pid == 0 ){
		ports[ port_num ].owner_pid = pid;
		ports[ port_num ].receive = receive;
		return 0;
	} else {
		// If someone owns the port, return the pid (non-0)
		return owner_pid;
	}
}

int32_t _release_port( uint16_t port_num, uint32_t pid ){
	// Determine who owns the port
	uint32_t owner_pid = ports[ port_num ].owner_pid;
	// If the owner is asking to release it, release it.  Otherwise, return
	// the owner pid
	if ( owner_pid == pid ){
		ports[ port_num ].owner_pid = 0;
		ports[ port_num ].receive = 0;
		return 0;
	} else {
		return owner_pid;
	}
}

void _receive( packet_t *p, uint8_t n ){
	// Keep track of the first packet in the string of received packets
	// to the same port
	packet_t *first_packet = p;
	// Keep track of port that packets are being read to
	uint16_t prev_port = 0;
	// Number of packets in a row going to the same port
	uint16_t count = 0;

	// For every packet, parse it and determine its destination port.
	// Call the receive function associated with the destination port
	for ( uint8_t i = 0; i < n; i++ ){
		// Convert the header so its readable

		ipv4_header_ntoh( &(p[i].header) );

		if ( p->header.protocol == UDP_PROTOCOL ){
			// Interpret the packet as a UDP packet
			udp_packet_t *u =  (udp_packet_t *)(&p[i]);
			udp_header_ntoh( &(u->udp_header) );

			// Determine the port
			uint16_t port = u->udp_header.dest_port;
			if ( port == prev_port ){
				// If its the same as the last packet, keep
				// track of how many in a row that is
				count++;
			} else {
				// if not, call the port's receive function
				ports[prev_port].receive( first_packet, count );
				first_packet = &p[i];
				count = 1;
			}
		}
	}

	// Make sure you receive all of the the packets
	ports[prev_port].receive( first_packet, count );
}

void send( packet_t *p, uint8_t n ){
	// Verify the outgoing IP is correct before sending, then send packets
	for ( uint8_t i = 0; i < n; ++i ){
		ipv4_header_ntoh( &(p[i].header) );
		ipv4_addr_cpy( &(p->header.source_ip), &(ipv4_addr_t){{0,0,0,0}} );
                for (uint8_t b = 0; b < PACKET_DUMP_LEN; ++b){
                        c_printf("%.2x", ((p_t *)p)->data[b]);
                }
		ipv4_header_hton( &(p[i].header) );
	}
}
