h18977
s 00076/00000/00000
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
** File:	scheduler.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	scheduler module declarations
*/

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "common.h"

#include "process.h"
#include "queue.h"

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

extern pcb_t *_current;		// the currently-running process
extern pcb_t *_idle_pcb;	// PCB for the idle process
extern pcb_t *_init_pcb;	// PCB for the init process
extern queue_t _ready;		// the RR ready queue

/*
** Prototypes
*/

/*
** _sched_init()
**
** initialize the scheduler module
*/

void _sched_init( void );

/*
** _sched(pcb)
**
** schedule a process for execution
*/

void _sched( pcb_t *pcb );

/*
** _dispatch()
**
** give the CPU to a process
**
** if there are no ready processes, dispatch the idle process
*/

void _dispatch( void );

#endif

#endif
E 1
