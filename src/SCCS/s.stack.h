h63205
s 00106/00000/00000
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
** File:	stack.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	definitions and prototypes for the stack module
*/

#ifndef _STACK_H_
#define _STACK_H_

#include "common.h"

#include "process.h"

/*
** General (C and/or assembly) definitions
*/

// number of user stacks, including ones for the idle process and the OS

#define	N_STACKS	(N_PROCS + 2)

// number of 32-bit unsigned integers in a stack

#define	STACK_SIZE	1024

#ifndef __SP_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/

// stack structure

typedef uint32_t stack_t[STACK_SIZE];

/*
** Globals
*/

extern stack_t _system_stack;	// the OS stack
extern uint32_t *_system_esp;	// the OS %ESP value

/*
** Prototypes
*/

/*
** _stk_init()
**
** initializes all stack-related data structures
*/

void _stk_init( void );

/*
** _stk_alloc()
**
** allocate a stack structure
**
** returns a pointer to the stack, or NULL on failure
*/

stack_t *_stk_alloc( void );

/*
** _stk_dealloc(stk)
**
** deallocate a stack, putting it into the list of available stacks
*/

void _stk_dealloc( stack_t *stk );

/*
** _stk_setup(stk,entry)
**
** set up an initial stack with a fresh user context
**
** returns:
**	pointe to the context save area in the stack
*/

context_t *_stk_setup( stack_t *stk, uint32_t entry );

/*
** _stk_dump(msg,stk)
**
** dump the contents of the provided stack, eliding
** duplicate lines
*/

void _stk_dump( char *msg, stack_t *stack );


#endif

#endif
E 1
