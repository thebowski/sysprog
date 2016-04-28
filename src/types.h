/*
** SCCS ID:	@(#)types.h	1.1	4/22/16
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
