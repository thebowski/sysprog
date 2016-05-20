/*
** SCCS ID:	@(#)syscall.h	1.1	4/22/16
**
** File:	syscall.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor: Matthew Cheman mnc3139
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
#define SYS_readfile 11
#define SYS_writefile 12
#define SYS_clearfiles 13
#define SYS_getgfxcontext 14
#define SYS_drawscreen 15
#define SYS_reqport     16
#define SYS_relport     17
#define SYS_send        18

#define	N_SYSCALLS	19

/* // dummy system call code to test the syscall ISR */

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
