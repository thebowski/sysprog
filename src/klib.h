/*
** SCCS ID:	@(#)klib.h	1.1	4/22/16
**
** File:	klib.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	kernel support library functions
*/

#ifndef _KLIB_H_
#define _KLIB_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Globals
*/

/*
** Prototypes
*/

/*
** _get_ebp - return current contents of EBP at the time of the call
**
** Could be used, e.g., by _kpanic to print a traceback
*/

uint32_t _get_ebp( void );

/*
** _put_char_or_code( ch )
**
** prints the character on the console, unless it is a non-printing
** character, in which case its hex code is printed
*/

void _put_char_or_code( int ch );

/*
** _khandlekey( char code )
**
** Process keypresses for special use by the kernel. If the code was used, returns 1 else 0
*/

int _khandlekey( char code );

/*
** _kmemset - initialize all bytes of a block of memory to a specific value
**
** usage:  _kmemclr( buffer, length, value )
*/

void _kmemset( register byte_t *buf, register uint32_t len, register uint8_t value );

/*
** _kmemclr - initialize all bytes of a block of memory to zero
**
** usage:  _kmemclr( buffer, length )
*/

void _kmemclr( register byte_t *buf, register uint32_t len );

/*
** _kmemcpy - copy a block from one place to another
**
** usage:  _kmemcpy( dest, src, length )
**
** may not correctly deal with overlapping buffers
*/

void _kmemcpy( register byte_t *dst, register byte_t *src, register uint32_t len );

/*
** _kstrcmp - compare two NUL-terminated strings
*/

int _kstrcmp( register const char *s1, register const char *s2 );

/*
** _kpanic - kernel-level panic routine
**
** usage:  _kpanic( mod, msg )
**
** Prefix routine for __panic() - can be expanded to do other things
** (e.g., printing a stack traceback)
*/

void _kpanic( char *mod, char *msg );

#endif

#endif
