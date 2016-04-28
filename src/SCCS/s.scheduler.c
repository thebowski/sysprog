h56822
s 00140/00000/00000
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
** File:	scheduler.c
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	implementation of the scheduler module
*/

#define	__SP_KERNEL__

#include "common.h"

#include "scheduler.h"
#include "process.h"
#include "queue.h"
#include "clock.h"

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

pcb_t *_current;	// the currently-running process
pcb_t *_idle_pcb;	// PCB for the idle process
pcb_t *_init_pcb;	// PCB for the init process
queue_t _ready;		// the RR ready queue

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*
** _sched_init()
**
** initialize the scheduler module
*/

void _sched_init( void ) {

	// initialize the ready queue

	_q_reset( &_ready, NULL );
	
	// report that we have finished

	c_puts( " SCHED" );
}

/*
** _sched(pcb)
**
** schedule a process for execution according to its priority
*/

void _sched( pcb_t *pcb ) {

	// sanity check - can't schedule "nothing"!

	if( pcb == NULL ) {
		// should this be an error?
		return;
	}

#ifdef DEBUG
	c_printf( "*** _sched() of PID %d\n", pcb->pid );
#endif

	// mark the process as ready to execute

	pcb->state = PS_READY;

	// if it isn't the idle process, put it on the ready queue
	// note: we provide the PID as a key, but _ready isn't
	// ordered, so that's there only for identification

	if( pcb != _idle_pcb ) {
		_q_insert( &_ready, pcb, (void *) pcb->pid );
#ifdef DEBUG
		_pcb_dump( "scheduled PCB", pcb );
		_context_dump( "scheduled context", pcb->context );
		__delay( 300 );
	} else { // idle()
		c_printf( "*** _sched() of idle() IGNORED\n" );
	// 	__delay( 100 );
#endif
	}
}

/*
** _dispatch()
**
** give the CPU to a process
**
** if there are no ready processes, dispatch the idle process
*/

void _dispatch( void ) {

	// pick the process to be dispatched

	if( _ready.size < 1 ) {
		_current = _idle_pcb;
	} else {
		_current = _q_remove( &_ready );
	}

#ifdef DEBUG
	c_printf( "*** _dispatch() of PID %d\n", _current->pid );
	// _pcb_dump( "dispatched PCB", _current );
	// _context_dump( "dispatched context", _current->context );
	// __delay( 300 );
#endif

	// mark it as the current process

	_current->state = PS_RUNNING;

	// give it a standard quantum

	_current->quantum = QUANT_STD;
}	
E 1
