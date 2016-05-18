#include "c_io.h"
#include "network.h"
#include "udp.h"
#include "ip.h"

uint32_t network_test(){
        c_printf("-------------------------- Test UDP creation and send stub\n");
        test_udp();
        return 0;
}

void receive( packet_t buf[], uint8_t size ){
        for ( int i = 0; i < size; ++i ){
                c_printf("-------------------------- Test UDP creation and send stub\n");
                for ( int b = 0; b < PACKET_DUMP_LEN; ++b ){
                        if ( b % 4 == 0 ){ c_printf( "   " ) };
                        if ( b % 16 == 0 ){ c_printf( "\n" ) };
                        c_printf( "%x ", buf[i].packet_data[b] );
                }
        }
}

void test_udp(){
        result = reqport( SEND_PORT, &receive );
        if ( result != 0 ){
                exit( 0 );
                return 0;
        }

        udp_packet_t buf[ TEST_BUF_L ];

        for ( int i  = 0; i < TEST_BUF_L; ++i ){
                new_ipv4_header( &buf[ i ].ip_header, (ip_addr_t){{127,0,0,1}},
                                 (ip_addr_t){{127,0,0,1}}, UDP_PROTOCOL,
                                 UDP_HEADER_L + 5, 64, FLAG_DF, 0, 0 );
                new_udp_header( &buf[ i ].udp_header, SEND_PORT, DEST_PORT,
                                5 );
                buf[ i ].udp_header.checksum = udp_checksum(
                        &buf[ i ].udp_header );
                buf[ i ].data = { 't', 'e', 's', 't', 0x0 };
        }
        c_printf("-------------------------- Sent packets\n");
        send( SEND_PORT, &buf, TEST_BUF_L );
        c_printf("-------------------------- Received packets\n");
        receive( &buf, TEST_BUF_L );
}
