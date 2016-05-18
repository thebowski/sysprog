#ifndef _NETWORK_TEST_H_
#define _NETWORK_TEST_H_

#define SEND_PORT 0xFEED
#define DEST_PORT 0xDEAF
#define TEST_BUF_L 2

#include "c_io.h"
#include "network.h"
#include "udp.h"
#include "ip.h"

/*
** network_test
**
** Simulates testing various network features
*/
int32_t network_test( void );

#endif
