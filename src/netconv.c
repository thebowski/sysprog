#include "netconv.h"

uint32_t htonl( uint32_t hostlong ){
	return ( (( hostlong >> 24 ) & 0xff)  |
		 (( hostlong >> 8 ) & 0xff00) |
		 (( hostlong << 8 ) & 0xff0000) |
		 (( hostlong << 24 ) & 0xff000000) );
}

uint16_t htons(uint16_t hostshort){
	return (( hostshort >> 8 ) | ( hostshort << 8 ));
}

uint32_t ntohl(uint32_t netlong){
	return ( (( netlong >> 24 ) & 0xff)  |
		 (( netlong >> 8 ) & 0xff00) |
		 (( netlong << 8 ) & 0xff0000) |
		 (( netlong << 24 ) & 0xff0000000) );
}

uint16_t ntohs(uint16_t netshort){
	return (( netshort >> 8 ) | ( netshort << 8 ));
}
