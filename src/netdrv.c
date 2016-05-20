#include "netdrv.h"
#include "c_io.h"
#include "syscall.h"
void _dev_send( packet_t buf[], uint16_t size ){
        c_printf( "Entered netdrv.c send routine\n" );
        _receive( buf, size );
}

void _dev_recieve( ){
}
