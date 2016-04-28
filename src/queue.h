/*
** SCCS ID:	@(#)queue.h	1.1	4/22/16
**
** File:	queue.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	declarations for the queue module
*/

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
**
** Our queues are generic, self-ordering queues.  Each queue has an
** associated ordering routine which is used by the insertion routine to
** order the queue elements.  This allows us to have different sorting
** criteria for different queues, but manage them with one set of functions.
*/

// prototype for the ordering function

typedef int32_t (*cmpf_t)( const void *, const void * );

// queue nodes (used internally to keep track of queue entries)
//
// this is anonymous; we must have a declaration for qnode_t in order
// to define queue_t, but we don't want other modules to fiddle with
// qnodes on their own

typedef struct qnode qnode_t;

// the queue structure

typedef struct queue {
	qnode_t *head;		// first entry in queue
	qnode_t *tail;		// last entry in queue
	uint32_t size;		// occupancy of queue
	cmpf_t cmp;		// comparison function
} queue_t;

/*
** Globals
*/

/*
** Prototypes
*/

/*
** _cmp_ascending_uint32()
**
** compare two key values as ascending-ordered 32-bit unsigned integers
**
** returns -1 if k1 < k2, 0 if k1 == k2, and +1 if k1 > k2
*/

int32_t _cmp_ascending_uint32( const void *k1, const void *k2 );

/*
** _q_init()
**
** initialize the queue module
*/

void _q_init( void );

/*
** _q_reset(que,cmp)
**
** reset the queue structure to its "empty" state,
** using 'cmp' as its comparison function
*/

void _q_reset( queue_t *que, cmpf_t cmp );

/*
** _q_insert(que,data,key)
**
** insert the supplied data value into the queue, ordering the
** queue using its built-in comparison function
*/

void _q_insert( queue_t *q, void *data, void *key );

/*
** _q_remove(que)
**
** remove the first element from the queue
**
** returns the thing that was removed, or NULL
*/

void *_q_remove( queue_t *q );

/*
** _q_remove_by_data(que,data)
**
** remove the element identified by the specified data value from the queue
**
** returns the data field from the entry, or NULL
*/

void *_q_remove_by_data( queue_t *q, void *data );

/*
** _q_remove_by_key(que,key)
**
** remove the element identified by the specified key from the queue
**
** returns the data field from the entry, or NULL
*/

void *_q_remove_by_key( queue_t *q, void *key );

/*
** _q_peek(que)
**
** peek at the data of the first element in the queue
**
** returns the data field, or NULL
*/

void *_q_peek( queue_t *q );

/*
** _q_kpeek(que)
**
** peek at the key of the first element in the queue
**
** returns the key, or NULL
*/

void *_q_kpeek( queue_t *q );

/*
** _q_dump(which,queue)
**
** dump the contents of the specified queue to the console
*/

void _q_dump( char *which, queue_t *q );

#endif

#endif
