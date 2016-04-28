/*
** SCCS ID:	@(#)system.h	1.1	4/22/16
**
** File:	system.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	miscellaneous OS support functions
*/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "common.h"
#include <x86arch.h>
#include "bootstrap.h"

#include "process.h"

// need do_exit()

#include "ulib.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP_ASM__

/*
** Start of C-only definitions
*/

// default contents of EFLAGS register

#define DEFAULT_EFLAGS  (EFLAGS_MB1 | EFLAGS_IF)

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
** _zombify(pcb,status)
**
** turn a process into a zombie, or give its status to
** a waiting parent
*/

void _zombify( pcb_t *pcb, uint32_t status );

/*
** _init - system initialization routine
**
** Called by the startup code immediately before returning into the
** first user process.
*/

void _init( void );

#endif

#endif
