h20140
s 00248/00000/00000
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
** File:	process.c
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	implementation of the process module
*/

#define	__SP_KERNEL__

#include "common.h"

#include "process.h"
#include "queue.h"

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

static queue_t _free_pcbs;	// all unallocated PCBs

/*
** PUBLIC GLOBAL VARIABLES
*/

pcb_t _pcbs[N_PCBS];		// all the PCBs in the system
uint32_t _next_pid;		// next available PID

queue_t _sleeping;		// sleep queue

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*
** _pcb_init()
**
** initializes all PCB-related data structures
*/

void _pcb_init( void ) {

	// initialize the "free PCB"  and "zombie" queues

	_q_reset( &_free_pcbs, NULL );

	// initialize the "sleeping processes" queue
	//
	// this one is an ordered queue

	_q_reset( &_sleeping, _cmp_ascending_uint32 );
	
	// put all the PCBs in the queue

	for( int i = 0; i < N_PROCS; ++i ) {
		_pcb_dealloc( &_pcbs[i] );
	}

	// set the initial PID
	// (must be the PID of the initial process)

	_next_pid = PID_INIT;

	// report that we have finished
	
	c_puts( " PCB" );
}

/*
** _pcb_cleanup(pcb)
**
** clean up and free a pcb and its associated stack
*/

void _pcb_cleanup( pcb_t *pcb ) {

	// sanity check

	if( pcb == NULL ) {
		return;
	}

	// if it has a stack, free it

	if( pcb->stack != NULL ) {
		_stk_dealloc( pcb->stack );
	}

	// release the PCB

	_pcb_dealloc( pcb );
}

/*
** _pcb_alloc()
**
** allocate a PCB structure
**
** returns a pointer to the PCB, or NULL on failure
*/

pcb_t *_pcb_alloc( void ) {
	pcb_t *new;
	
	// if there aren't any, can't allocate one!

	if( _free_pcbs.size < 1 ) {
		return( NULL );
	}
	
	// pull one off the free queue and make it "new"

	new = (pcb_t *) _q_remove( &_free_pcbs );
	new->state = PS_NEW;
	
	// send it back to the caller

	return( new );
}

/*
** _pcb_dealloc(pcb)
**
** deallocate a PCB, putting it into the set of available PCBs
*/

void _pcb_dealloc( pcb_t *pcb ) {

	// sanity check:  avoid deallocating a NULL pointer

	if( pcb == NULL ) {
		// should this be an error?
		return;
	}

	// make sure it's marked as available

	pcb->state = PS_UNUSED;

	// question:  should we clear the other fields here?

	// tuck it away for future use

	_q_insert( &_free_pcbs, pcb, 0 );
}

/*
** _pcb_find(pid)
**
** locate the PCB having the specified PID
**
** returns a pointer to the PCB, or NULL if not found
*/

pcb_t *_pcb_find( int32_t pid ) {

#ifdef DEBUG
	c_printf( "*** _pcb_find(%d)\n", pid );
#endif

	for( int i = 0; i < N_PCBS; ++i ) {
		if( _pcbs[i].state >= PS_FIRST_LIVE &&
		    _pcbs[i].pid == pid ) {
			return( &_pcbs[i] );
		}
	}

#ifdef DEBUG
	c_printf( "*** _pcb_find(%d) FAILED\n", pid );
#endif

	return( NULL );
}

/*
** _pcb_dump(pcb)
**
** dump the contents of this PCB to the console
*/

void _pcb_dump( const char *which, pcb_t *pcb ) {

	c_printf( "%s @%08x: ", which, (uint32_t) pcb );
	if( pcb == NULL ) {
		c_puts( " NULL???\n" );
		return;
	}

	c_printf( " pids %d/%d state ", pcb->pid, pcb->ppid );
	switch( pcb->state ) {
		case PS_UNUSED:		c_puts( "UNUSED" ); break;
		case PS_NEW:		c_puts( "NEW" ); break;
		case PS_READY:		c_puts( "READY" ); break;
		case PS_RUNNING:	c_puts( "RUNNING" ); break;
		case PS_SLEEPING:	c_puts( "SLEEPING" ); break;
		case PS_BLOCKED:	c_puts( "BLOCKED" ); break;
		case PS_ZOMBIE:		c_puts( "ZOMBIE" ); break;
		default:		c_printf( "? (%d)", pcb->state );
	}

	c_printf( "\n q %d wake %08x", (int) pcb->quantum, pcb->wakeup );

	c_printf( " context %08x stack %08x\n",
		  (uint32_t) pcb->context, (uint32_t) pcb->stack );
}

/*
** _context_dump(context)
**
** dump the contents of this context to the console
*/

void _context_dump( const char *which, context_t *context ) {

	c_printf( "%s @%08x: ", which, (uint32_t) context );
	if( context == NULL ) {
		c_puts( " NULL???\n" );
		return;
	}

	c_printf( "\n     ss %08x  gs %08x  fs %08x  es %08x\n",
		context->ss, context->gs, context->fs, context->es );
	c_printf( "     ds %08x edi %08x esi %08x ebp %08x\n",
		context->ds, context->edi, context->esi, context->ebp );
	c_printf( "    esp %08x ebx %08x edx %08x ecx %08x\n",
		context->esp, context->ebx, context->edx, context->ecx );
	c_printf( "    eax %08x vec %08x cod %08x eip %08x\n",
		context->eax, context->vector, context->code, context->eip );
	c_printf( "     cs %08x efl %08x\n",
		context->cs, context->eflags );

}
E 1
