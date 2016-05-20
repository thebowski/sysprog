/*
** Author:    Alexander Bobowski
** Class:     Systems Programming
** Professor: Prof. Carithers
** Date:      5/19/2015
*/

#include "netdrv.h"
#include "c_io.h"
#include "syscall.h"

void _dev_send( packet_t buf[], uint16_t size ){
        c_printf( "Entered netdrv.c send routine\n" );
        _receive( buf, size );
}

void _dev_recieve( ){
}
