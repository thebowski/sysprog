/*
** Author:    Alexander Bobowski
** Class:     Systems Programming
** Professor: Prof. Carithers
** Date:      5/19/2016
*/


#ifndef _NETCONV_H_
#define _NETCONV_H_

#include "types.h"

/*
** Converts byte order for network transmission for 32 bit values
**
** Arguments:
**     uint32_t hostlong - A 32-bit integer in host byte order
** Returns:
**     uint43_t ---------- A 32-bit integer in network byte order
*/
uint32_t htonl(uint32_t hostlong);

/*
** Converts byte order for network transmission for 16 bit values
**
** Arguments:
**     uint16_t hostshort - A 16-bit integer in host byte order
** Returns:
**     uint16_t ----------- A 16-bit integer in network byte order
*/
uint16_t htons(uint16_t hostshort);

/*
** Converts byte order for host parsing for 32 bit values
**
** Arguments:
**     uint32_t hostlong - A 32-bit integer in network byte order
** Returns:
**     uint32_t ---------- A 32-bit integer in host byte order
*/
uint32_t ntohl(uint32_t netlong);

/*
** Converts byte order for host parsing for 16 bit values
**
** Arguments:
**     uint16_t hostlong - A 16-bit integer in network byte order
** Returns:
**     uint16_t ---------- A 16-bit integer in hostx byte order
*/
uint16_t ntohs(uint16_t netshort);

#endif
