/*
** SCCS ID:	@(#)process.h	1.1	4/22/16
**
** File:	process.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	definitions and prototypes for the process module
*/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

// number of user PCBs, including one for the idle process

#define	N_PCBS		(N_PROCS + 1)

// process states

#define	PS_UNUSED	0
#define	PS_NEW		1
#define	PS_READY	2
#define	PS_RUNNING	3
#define	PS_SLEEPING	4
#define	PS_BLOCKED	5
#define	PS_WAITING	6
#define	PS_ZOMBIE	7

// in case we need to know if a process is living

#define	PS_FIRST_LIVE	PS_NEW
#define	PS_LAST_LIVE	PS_WAITING

#define	N_STATES	8

// PID of the initial user process

#define	PID_INIT	1

#ifndef __SP_ASM__

/*
** Start of C-only definitions
*/

// REG(pcb,x) -- access a specific register in process context

#define	REG(pcb,x)	((pcb)->context->x)

// RET(pcb) -- access return value register in process context

#define	RET(pcb)	((pcb)->context->eax)

// ARG(pcb,n) -- access argument #n from the indicated process
//
// ARG(pcb,0) --> return address
// ARG(pcb,1) --> first actual parameter
// ARG(pcb,2) --> second actual parameter
//
// IF THE PARAMETER PASSING MECHANISM CHANGES, SO MUST THIS

#define	ARG(pcb,n)	( ( (uint32_t *) (((pcb)->context) + 1) ) [(n)] )

// number of ports able to be associated with a process
#define PS_MAX_PORTS    8

/*
** Types
*/

// process context structure
//
// NOTE:  the order of data members here depends on the
// register save code in isr_stubs.S!!!!

typedef struct context {
	uint32_t ss;
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t vector;
	uint32_t code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
} context_t;

// include these here after the context definition

#include "stack.h"
#include "queue.h"

// process control block
//
// members are ordered by size

typedef struct pcb {
	// 32-bit fields
	context_t	*context;	// context save area pointer
	stack_t		*stack;		// per-process runtime stack
	uint32_t	wakeup;		// for sleeping processes
	int32_t		pid;		// our pid
	int32_t		ppid;		// out parent's pid

	// 16-bit fields
	uint16_t        ports[ PS_MAX_PORTS ];  // The ports associated
                                                // With the process
	// 8-bit fields
	uint8_t		state;		// current process state
	uint8_t		quantum;	// remaining execution quantum
} pcb_t;

/*
** Globals
*/

extern pcb_t _pcbs[];		// all PCBs in the system
extern uint32_t _next_pid;	// next available PID

extern queue_t _sleeping;	// sleep queue

/*
** Prototypes
*/

/*
** _pcb_init()
**
** initializes all PCB-related data structures
*/

void _pcb_init( void );

/*
** _pcb_cleanup(pcb)
**
** clean up and free a pcb and its associated stack
*/

void _pcb_cleanup( pcb_t *pcb );

/*
** _pcb_alloc()
**
** allocate a PCB structure
**
** returns a pointer to the PCB, or NULL on failure
*/

pcb_t *_pcb_alloc( void );

/*
** _pcb_dealloc(pcb)
**
** deallocate a PCB, putting it into the list of available PCBs
*/

void _pcb_dealloc( pcb_t *pcb );

/*
** _pcb_find(pid)
**
** locate the PCB having the specified PID
**
** returns a pointer to the PCB, or NULL if not found
*/

pcb_t *_pcb_find( int32_t pid );

/*
** _pcb_dump(pcb)
**
** dump the contents of this PCB to the console
*/

void _pcb_dump( const char *which, pcb_t *pcb );

/*
** _context_dump(pcb)
**
** dump the contents of this process context to the console
*/

void _context_dump( const char *which, context_t *context );

#endif

#endif
