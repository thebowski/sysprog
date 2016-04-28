h13634
s 00072/00000/00000
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
E 1
