h59299
s 00060/00000/00000
d D 1.1 16/04/22 16:40:46 wrc 1 0
c date and time created 16/04/22 16:40:46 by wrc
e
u
U
f e 0
t
T
I 1
/*
** SCCS ID:	%W%	%G%
**
** File:	types.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	General data type declarations
*/

#ifndef _TYPES_H_
#define _TYPES_H_

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

// size-specific integer types

typedef char		int8_t;
typedef unsigned char	uint8_t;
typedef uint8_t		byte_t;

typedef short		int16_t;
typedef unsigned short	uint16_t;

typedef long		int32_t;
typedef unsigned long	uint32_t;

#ifdef __SP_KERNEL__

/*
** OS-only type declarations
*/

#endif

/*
** Globals
*/

/*
** Prototypes
*/

#endif

#endif
E 1
