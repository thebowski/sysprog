h30683
s 00093/00000/00000
d D 1.1 16/04/22 16:40:45 wrc 1 0
c date and time created 16/04/22 16:40:45 by wrc
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
** File:	syscall.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	system call declarations
*/

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

// system call codes

#define	SYS_exit	0
#define	SYS_readch	1
#define	SYS_writes	2
#define	SYS_fork	3
#define	SYS_exec	4
#define	SYS_wait	5
#define	SYS_kill	6
#define	SYS_sleep	7
#define	SYS_getpid	8
#define	SYS_getppid	9
#define	SYS_gettime	10

#define	N_SYSCALLS	11

// dummy system call code to test the syscall ISR

#define	SYS_bogus	(N_SYSCALLS + 100)

// system call interrupt vector number

#define	INT_VEC_SYSCALL	0x80

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

extern queue_t _zombie;		// "zombie" processes
extern queue_t _waiting;	// "waiting for child" processes

/*
** Prototypes
*/

#ifdef __SP_KERNEL__

/*
** OS-only definitions
*/

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** Prototypes
*/

/*
** _sys_init()
**
** initializes all syscall-related data structures
*/

void _sys_init( void );

#endif

#endif

#endif
E 1
