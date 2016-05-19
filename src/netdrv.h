#ifndef _NETDRV_H_
#define _NETDRV_H_

#include "types.h"
#include "network.h"
#include "ip.h"

/*
** _dev_send
**
** Pretends to send the packets.  Actually just sends the packets back to the
** parsing function.
**
** Aguments:
**     packet_t buf[] - A buffer of packets to send
**     uint16_t size -- The number of packets in the buffer
*/
void _dev_send( packet_t buf[], uint16_t size );

/*
** _dev_receive
**
** A stub for the driver receive function.  Does nothing at all
*/
void _dev_recieve( void );

#endif
