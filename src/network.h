#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "ip.h"
#include "udp.h"
#include "icmp.h"
#include "c_io.h"
#include "netdrv.h"

#define PACKET_DUMP_LEN 150

#define MAX_PORTS 0xFFFF
#define PACKET_BUF_L 0xFFF

packet_t packet_buf[ PACKET_BUF_L ];
uint16_t packet_buf_index;

/*
** receive_t
**
** Defines what it means to be a function pointer to a receive function.
**
** Arguments:
**     packet_t packets[] - A buffer of packets recieved
**     uint8_t num_packet - The number of packets in the buffer
*/
typedef void (*receive_t)( packet_t[], uint8_t );

/*
** port
** The data structure describing a port.
**
** Fields:
**     uint32_t owner_pid - Indicates what process owns the port
**     void receive_t ----- A pointer to the process' receive function
*/
typedef struct port {
	uint32_t owner_pid;
	receive_t receive;
} port_t;

/*
** The ports on the system.  The functions included in this file manipulate
** the list of ports and serve as go-betweens to the functions in the driver
** for the network card
*/
port_t ports[ MAX_PORTS ];

/*
** network_init
**
** Initialized the network structures
*/
void _network_init( void );

/*
** _request_port
**
** Attempts to assign the specified port to the specified process.
**
** Arguments:
**     uint16_t port_num - Port number to assign
**     receive_t receive - Pointer to function to receive packet
**     uint32_t pid ------ ID of the process to assign the port to
** Returns:
**     non-0 if successful, 0 otherwise
*/
int32_t _request_port( uint16_t port_num, receive_t receive, uint32_t pid );

/*
** _release_port
**
** Attempts to release the specified port from the specified process
**
** Arguments:
**     uint16_t port_num - The number of the port to release
**     uint32_t pid ------ The process id of the calling process
** Returns:
**     non-0 if successful, 0 otherwise
*/
int32_t _release_port( uint16_t port_num, uint32_t pid );

/*
** _receive
**
** Called by the driver, distributes received packets to ports
**
** Arguments:
**     packet_t p[] - A pointer to the packet the buffer of packets received
**                    by the network card
**     uint8_t n ---- Number of packets in the buffer
*/
void _receive( packet_t p[], uint8_t n );

/*
** _send
**
** Called by the driver, distributes received packets to ports
**
** Arguments:
**     packet_t p[] - A pointer to the packet the buffer of packets to be sent
**                    by the network card
**     uint8_t n ---- Number of packets in the buffer
*/
void _send( packet_t p[], uint8_t n );

#endif
