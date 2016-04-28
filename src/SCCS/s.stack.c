h32883
s 00277/00000/00000
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
** File:	stack.c
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	implementation of the stack module
*/

#define	__SP_KERNEL__

#include "common.h"

#include "stack.h"
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

static queue_t _free_stacks;		// all unallocated stacks
static stack_t _stacks[N_STACKS];	// all the stacks in the system

/*
** PUBLIC GLOBAL VARIABLES
*/

stack_t _system_stack;		// the OS stack
uint32_t *_system_esp;		// the OS %ESP value

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*
** _stk_init()
**
** initializes all stack-related data structures
*/

void _stk_init( void ) {
	
	// initialize the "free stacks" queue

	_q_reset( &_free_stacks, NULL );

	// put all the stacks in the queue

	for( int i = 0; i < N_STACKS; ++i ) {
		_stk_dealloc( &_stacks[i] );
	}
	
	// report that we have finished

	c_puts( " STK" );
}

/*
** _stk_alloc()
**
** allocate a stack structure
**
** returns a pointer to the stack, or NULL on failure
*/

stack_t *_stk_alloc( void ) {
	stack_t *new;

	// if there aren't any, can't allocate one!

	if( _free_stacks.size < 1 ) {
		return( NULL );
	}

	// pull one off the free queue

	new = (stack_t *) _q_remove( &_free_stacks );

	// send it back to the caller

	return( new );
}

/*
** _stk_dealloc(stack)
**
** deallocate a stack, putting it into the set of available stacks
*/

void _stk_dealloc( stack_t *stack ) {
	
	// sanity check:  avoid deallocating a NULL pointer

	if( stack == NULL ) {
		// should this be an error?
		return;
	}

	// question:  should we clear the stack here?

	// tuck it away for future use

	_q_insert( &_free_stacks, (void *) stack, 0 );
}

/*
** _stk_setup(stk,entry)
**
** set up an initial stack with a fresh user context
**
** returns:
**	pointe to the context save area in the stack
*/

context_t *_stk_setup( stack_t *stack, uint32_t entry ) {
	uint32_t *ptr;
	context_t *context;

	// sanity check!

	if( stack == NULL ) {
		return( NULL );
	}

	// clear the whole stack

	_kmemclr( (void *) stack, sizeof(stack_t) );

	/*
	** Set up the initial stack contents for a (new) user process.
	**
	** We reserve one longword at the bottom of the stack as
	** scratch space.  Above that, we simulate a call from do_exit()
	** by pushing the address of the entry point of do_exit() as a
	** "return address".  Above that, we place an context_t area
	** that is initialized with the standard initial register contents.
	**
	** The low end of the stack will contain these values:
	**
	**      esp ->  ?        <- context save area
	**              ...      <- context save area
	**              ?        <- context save area
	**              do_exit  <- return address for faked call to main()
	**              0        <- last word in stack
	**
	** When this process is dispatched, the context restore code
	** will pop all the saved context information off the stack,
	** leaving the "return address" on the stack as if the main()
	** for the process had been "called" from the do_exit() function.
	** When main() returns, it will "return" to the entry point of
	** do_exit(), which will clean it up.
	*/

	// first, find the address following the stack

	ptr = (uint32_t *) (stack + 1);

	// assign the filler data

	*--ptr = 0;  // for testing, try 0xbeefcafe

	// assign the return address for the faked call to main()

	*--ptr = (uint32_t) do_exit;

	// next, set up the process context

	context = ((context_t *) ptr) - 1;

	// initialize all the fields that should be non-zero, starting
	// with the entry point

	context->eip = entry;

	// EFLAGS must be set up to re-enable IF when we switch
	// "back" to this context

	context->eflags = DEFAULT_EFLAGS;

	// finally, the segment registers

	context->cs = GDT_CODE;
	context->ss = GDT_STACK;
	context->ds = GDT_DATA;
	context->es = GDT_DATA;
	context->fs = GDT_DATA;
	context->gs = GDT_DATA;

	// all done - return the context pointer

	return( context );
}

/*
** _stk_dump(msg,stk)
**
** dump the contents of the provided stack, eliding
** duplicate lines
**
** assumes the stack size is a multiple of four words
*/

// space for four words & 8+2 bytes each, plus a NUL, and some extra

#define	TMPBUFSZ	50

void _stk_dump( char *msg, stack_t *stack ) {
	int words = sizeof(stack_t) / 4;
	int eliding = 0;
	char oldbuf[TMPBUFSZ], buf[TMPBUFSZ];
	uint32_t addr = (uint32_t )stack;
	uint32_t *sp = (uint32_t *) stack;
	char hexdigits[] = "0123456789ABCDEF";

	c_printf( "*** dumping %s:\n", msg );

	// _kmemset( (uint8_t *) oldbuf, TMPBUFSZ, 0 );
	oldbuf[0] = '\0';

	while( words > 0 ) {
		char *bp = buf;
		uint32_t start_addr = addr;

		for( int i = 0; i < 4; ++i ) {
			uint32_t data = *sp++;

			*bp++ = ' ';
			*bp++ = ' ';

			for( int j = 0; j < 8; ++j ) {
				uint32_t value = data & 0xf0000000;
				value = (value >> 28) & 0xf;
				*bp++ = hexdigits[value];
				data <<= 4;
			}
		}
		*bp = '\0';

		words -= 4;
		addr += 16;

		// if this line looks like the last one, skip it

		if( _kstrcmp(oldbuf,buf) == 0 ) {
			++eliding;
			continue;
		}

		// it's different, so print it

		// start with the address
		c_printf( "%08x%c", start_addr, eliding ? '*' : ' ' );
		eliding = 0;

		// print the words
		c_printf( "%s\n", buf );

		// remember this line
		for( int i = 0; i < TMPBUFSZ; ++i )
			oldbuf[i] = buf[i];
	}
}
E 1
