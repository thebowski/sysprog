/*
** SCCS ID:	@(#)ulibc.c	1.1	4/22/16
**
** File:	ulibc.c
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	C implementations of user-level library functions
*/

#include "common.h"

#include "ulib.h"

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*
** strlen()
**
** return length of a NUL-terminated string
*/

uint32_t strlen( char *msg ) {
	uint32_t len = 0;

	while( *msg++ ) {
		++len;
	}

	return( len );
}

/*
** cvt_dec0
**
** local support routine for cvt_dec()
**
** copied from c_io.c cvtdec0() routine
*/

char *cvt_dec0( char *buf, int value ) {
	int quotient;

	quotient = value / 10;
	if( quotient < 0 ) {
		quotient = 214748364;
		value = 8;
	}
	if( quotient != 0 ) {
		buf = cvt_dec0( buf, quotient );
	}
	*buf++ = value % 10 + '0';
	return buf;
}

/*
** cvt_dec
**
** convert a 32-bit signed value into a
** NUL-terminated character string
**
** returns the number of characters placed into the buffer
** (not including the NUL)
**
** copied from c_io.c cvtdec() routine
*/

int cvt_dec( char *buf, int32_t value ) {
	char *bp = buf;

	if( value < 0 ) {
		*bp++ = '-';
		value = -value;
	}

	bp = cvt_dec0( bp, value );
	*bp  = '\0';

	return( bp - buf );
}

/*
** cvt_hex
**
** convert a 32-bit value into an (up to) 8-character
** NUL-terminated character string
**
** returns the number of characters placed into the buffer
** (not including the NUL)
**
** copied from c_io.c cvthex() routine
*/

int cvt_hex( char *buf, uint32_t value ) {
	char hexdigits[] = "0123456789ABCDEF";
	int chars_stored = 0;

	for( int i = 0; i < 8; i += 1 ) {
		int val = value & 0xf0000000;
		if( chars_stored || val != 0 || i == 7 ) {
			++chars_stored;
			val = (val >> 28) & 0xf;
			*buf++ = hexdigits[val];
		}
		value <<= 4;
	}

	*buf = '\0';

	return( chars_stored );
}

/*
** creadch()
**
** read a single character from the console
*/

int32_t creadch( void ) {
	return( readch(CHAN_CONSOLE) );
}

/*
** sreadch()
**
** read a single character from the SIO
*/

int32_t sreadch( void ) {
	return( readch(CHAN_SIO) );
}

/*
** cwrite()
**
** write a NUL-terminated buffer to the console
*/

int32_t cwrite( char *buf ) {
	return( writes(CHAN_CONSOLE,buf,strlen(buf)) );
    return 0;
}

/*
** swrite()
**
** write a NUL-terminated buffer to the SIO
*/

int32_t swrite( char *buf ) {
	return( writes(CHAN_SIO,buf,strlen(buf)) );
}

/*
** cwrites()
**
** write a sized buffer to the console
*/

int32_t cwrites( char *buf, int size ) {
	return( writes(CHAN_CONSOLE,buf,size) );
}

/*
** swrites()
**
** write a sized buffer to the SIO
*/

int32_t swrites( char *buf, int size ) {
	return( writes(CHAN_SIO,buf,size) );
}

/*      
** prt_status - print a status value to the console
**      
** the 'msg' argument is printed, followed by
** a hex representation of the 'stat' argument
*/

void prt_status( char *msg, uint32_t stat ) {
	char buf[8];

	if( msg == NULL ) {
		return;
	}

	cwrites( msg, strlen(msg) );

	int n = cvt_hex( buf, stat );

	cwrites( buf, n );
	cwrites( "\n", 1 );

}
