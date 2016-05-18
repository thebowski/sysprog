#include "network_test.h"
#include "syscall.h"
#include "common.h"

void receive( packet_t buf[], uint8_t size );

void test_udp( void );

int32_t network_test(){
        c_printf("-------------------------- Test UDP creation and send stub\n");
        test_udp();
        return 0;
}

void receive( packet_t buf[], uint8_t size ){
        for ( int i = 0; i < size; ++i ){
                c_printf("-------------------------- Test UDP creation and send stub\n");
                for ( int b = 0; b < PACKET_DUMP_LEN; ++b ){
                        if ( b % 4 == 0 ){ c_printf( "   " ); };
                        if ( b % 16 == 0 ){ c_printf( "\n" ); };
                        c_printf( "%x ", buf[i].packet_data[b] );
                }
        }
}

void test_udp(){
        uint32_t result = reqport( SEND_PORT, &receive );
        if ( result != 0 ){
                return;
        }

        udp_packet_t buf[ TEST_BUF_L ];

        for ( int i  = 0; i < TEST_BUF_L; ++i ){
                new_ipv4_header( &buf[ i ].ip_header, (ipv4_addr_t){{127,0,0,1}},
                                 (ipv4_addr_t){{127,0,0,1}}, UDP_PROTOCOL,
                                 UDP_HEADER_L + 5, 64, FLAG_DF, 0, 0 );
                new_udp_header( &buf[ i ].udp_header, SEND_PORT, DEST_PORT,
                                5 );
                buf[ i ].udp_header.checksum = udp_checksum(
                        &buf[ i ], &(ipv4_addr_t){{127,0,0,1}},
                        &(ipv4_addr_t){{127,0,0,1}} );
                buf[ i ].data[0] = 't';
                buf[ i ].data[0] = 'e';
                buf[ i ].data[0] = 's';
                buf[ i ].data[0] = 't';
                buf[ i ].data[0] = 0;

        }
        c_printf("-------------------------- Sent packets\n");
        send( (packet_t *)&buf[0], TEST_BUF_L );
        c_printf("-------------------------- Received packets\n");
        _receive( (packet_t *)&(buf[0]), TEST_BUF_L );
}
