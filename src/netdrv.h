#ifndef _NETDRV_H_
#define _NETDRV_H_

#include "types.h"
#include "network.h"
#include "ip.h"

void _dev_send( packet_t buf[], uint16_t size );

void _dev_recieve( void );

#endif
