#include "netdrv.h"


void _dev_send( packet_t buf[], uint16_t size ){
        _receive( buf, size );
}

void _dev_recieve( ){
}
