/*
** SCCS ID:	@(#)common.h	1.1	4/22/16
**
** File:	common.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	standard declarations needed by the OS and user code
*/

#ifndef _COMMON_H_
#define _COMMON_H_

// correct (?) way to define NULL

#define NULL		((void *) 0)

// maximum number of processes the system will support

#define	N_PROCS		25

// i/o channels

#define	CHAN_CONSOLE	0
#define	CHAN_SIO	1

// process exit codes

#define	EXIT_SUCCESS	0
#define	EXIT_FAILURE	1
#define	EXIT_KILLED	2
#define	EXIT_BAD_CODE	35

// error return values

#define	SUCCESS		0
#define	E_SUCCESS	0
#define	E_NO_PROCS	(-1)
#define	E_NO_CHAR	(-2)
#define	E_BAD_CHAN	(-3)

#ifndef __SP_ASM__

// only pull these in if we're not in assembly language

#include "types.h"

#ifdef __SP_KERNEL__

// OS needs the system headers and the kernel library headers

#include "c_io.h"
#include "support.h"
#include "system.h"

#include "klib.h"

#else

// User code needs only the user library headers

#include "ulib.h"

#endif

#endif

#endif
