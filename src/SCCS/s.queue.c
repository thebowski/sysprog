h43836
s 00599/00000/00000
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
** File:	queue.c
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	implementation of the queue module
*/

#define	__SP_KERNEL__

#include "common.h"

#include "queue.h"
#include "process.h"
#include "stack.h"

/*
** PRIVATE DEFINITIONS
*/

// number of qnodes to create
//
// need one per PCB and one per stack, plus a fudge factor

#define	N_QNODES	(N_PCBS + N_STACKS + 5)

/*
** PRIVATE DATA TYPES
*/

// structure of a qnode

struct qnode {
	void *key;		// sorting key
	void *data;		// queued data
	qnode_t *next;		// link to successor
};

/*
** PRIVATE GLOBAL VARIABLES
*/

static qnode_t _qnodes[N_QNODES];	// all qnodes in the system
static qnode_t *_free_qnodes;		// list of available qnodes

/*
** PUBLIC GLOBAL VARIABLES
*/

/*
** PRIVATE FUNCTIONS
*/

/*
** _qn_alloc()
**
** allocate a free qnode
**
** on success, returns a pointer to the qnode
**
** on failure, panics - this is a non-recoverable error
*/

static qnode_t *_qn_alloc( void ) {
	qnode_t *new = NULL;

	// if there aren't any, we're screwed

	if( _free_qnodes == NULL ) {
		_kpanic( "_qn_alloc", "no free qnodes" );
	}
	
	// grab the first one from the list

	new = _free_qnodes;

	// remove it from the list
	_free_qnodes = new->next;
	new->next = NULL;
	
	// send it home
	return( new );
}

/*
** _qn_dealloc()
**
** return a qnode to the free list
*/

static void _qn_dealloc( qnode_t *old ) {

	// make sure we only do this if we actually got one!

	if( old != NULL ) {

		// clear the other fields of the qnode

		old->data = old->key = NULL;

		// this is the new "first" qnode in the list
		old->next = _free_qnodes;
		_free_qnodes = old;
	}
}

/*
** _qn_init()
**
** initialize the list of available qnodes
**
** SHOULD ONLY BE CALLED IF NO QUEUES OR QNODES ARE CURRENTLY IN USE
*/

static void _qn_init( void ) {

	// we have a deallocator, so let's use it

	for( int i = 0; i < N_QNODES; ++i ) {
		_qn_dealloc( &_qnodes[i] );
	}

}

/*
** PUBLIC FUNCTIONS
*/

/*
** _cmp_ascending_uint32()
**
** compare two key values as ascending-ordered 32-bit unsigned integers
**
** returns -1 if k1 < k2, 0 if k1 == k2, and +1 if k1 > k2
*/

int32_t _cmp_ascending_uint32( const void *k1, const void *k2 ) {
	uint32_t uk1 = (uint32_t) k1;
	uint32_t uk2 = (uint32_t) k2;

	if( uk1 < uk2 )
		return( -1 );
	else if( uk1 == uk2 )
		return( 0 );

	return( 1 );
}

/*
** _q_init()
**
** initialize the queue module
*/

void _q_init( void ) {

	// set up the qnode list

	_qn_init();
	
	// report that we're done

	c_puts( " QUEUE" );
}

/*
** _q_reset(que,cmp)
**
** reset the queue structure to its "empty" state,
** using 'cmp' as its comparison function
*/

void _q_reset( queue_t *q, cmpf_t cmp ) {

	// reset the queue's fields

	q->head = q->tail = NULL;
	q->size = 0;
	q->cmp = cmp;

}

/*
** _q_insert(que,data,key)
**
** insert the supplied data value into the queue, ordering the
** queue using its built-in comparison function
*/

void _q_insert( queue_t *q, void *data, void *key ) {
	qnode_t *new, *prev, *curr;
	
	// sanity check:  make sure we got a queue!

	if( q == NULL ) {
		_kpanic( "_q_insert", "NULL queue parameter" );
	}

	// sanity check:  make sure we don't queue up a null pointer
	//
	// this is a concession to the fact that we don't have an
	// out-of-band method for signalling errors when removing
	// things from queues - typically, we just return NULL,
	// which won't work if we can queue up a NULL pointer.
	//
	// as a result, we can't queue up the integer 0.

	if( data == NULL ) {
		_kpanic( "_q_insert", "attempt to queue NULL" );
	}
	
	// allocate a qnode and fill it in

	new = _qn_alloc();
	new->data = data;
	new->key = key;
	
	// if this is the first entry, just add the node

	if( q->size == 0 ) {
		q->head = q->tail = new;
		q->size = 1;
		return;
	}
	
	// if this is an unordered queue, just append the node

	if( q->cmp == NULL ) {
		q->tail->next = new;
		q->tail = new;
		q->size += 1;
		return;
	}
	
	// ordered queue; traverse until we find the insertion point

	prev = NULL;
	curr = q->head;
	while( curr && q->cmp(key,curr->key) >= 0 ) {
		prev = curr;
		curr = curr->next;
	}
	
	/*
	** after the traversal, here are the four possible states:
	**
	** prev curr	meaning
	** ==== ====	=======
	**   0    0	empty list - can't happen
	**   0   !0	insert at front:  n->c, h->n
	**  !0   !0	insert in middle: n->c, p->n
	**  !0    0	append:  p->n, t->n
	*/

	if( prev == NULL ) {

		// if both pointers are NULL, we have a problem

		if( curr == NULL ) {
			_kpanic( "_q_insert", "empty queue, size != 0" );
		}

		// insert at front

		new->next = curr;
		q->head = new;

	} else if( curr == NULL ) {

		// append to the queue

		prev->next = new;
		q->tail = new;

	} else {

		// insert into the middle

		new->next = curr;
		prev->next = new;

	}
	
	// bump the size

	q->size += 1;
}

/*
** _q_remove(que)
**
** remove the first element from the queue
**
** returns the thing that was removed, or NULL
*/

void *_q_remove( queue_t *q ) {
	qnode_t *temp;
	void *data;
	
	// sanity check!

	if( q == NULL ) {
		_kpanic( "_q_remove", "NULL queue parameter" );
	}
	
	// can't remove from an empty queue

	if( q->size == 0 ) {
		return( NULL );
	}
	
	// pull the first entry off the queue

	temp = q->head;
	if( temp == NULL ) {
		// oops!
		_kpanic( "_q_remove", "NULL queue head, but size > 0" );
	}
	
	// unlink this entry from the queue

	q->head = temp->next;
	q->size -= 1;

	// remember the data field
	
	data = temp->data;
	
	// if this was the only node, make sure the queue is consistent

	if( q->head == NULL ) {

		// size should now be 0

		if( q->size != 0 ) {
			_kpanic( "_q_remove", "size > 0 empty result queue" );
		}

		// force the tail pointer to be NULL, too

		q->tail = NULL;

	} else if( q->size == 0 ) {

		// non-NULL head means there should be something here

		_kpanic( "_q_remove", "size 0 nonempty result queue" );

	}
			
	// get rid of the qnode

	_qn_dealloc( temp );
	
	// send the data back to the caller

	return( data );
}

/*
** _q_remove_by_key(que,key)
**
** remove the element identified by the specified key from the queue
**
** returns the data field from the entry, or NULL
*/

void *_q_remove_by_key( queue_t *q, void *key ) {
	qnode_t *prev, *curr;
	void *result;
	
	// sanity check!

	if( q == NULL ) {
		_kpanic( "_q_remove_by_key", "NULL queue parameter" );
	}
	
	// is there anything to remove?

	if( q->size < 1 ) {
		return( NULL );
	}
	
	// non-empty queue; locate the specified element

#ifdef DEBUG
	c_printf( "*** _q_remove_by_key: scanning %d for %08x\n",
		  q->size, key );
#endif

	prev = NULL;
	curr = q->head;
	while( curr != NULL && curr->key != key ) {
#ifdef DEBUG
		c_printf( " skip %08x", curr->key );
#endif
		prev = curr;
		curr = curr->next;
	}
#ifdef DEBUG
	c_putchar( '\n' );
#endif

	// did we find it?

	if( curr == NULL ) {
		return( NULL );
	}

	// yes - copy out the data pointer

	result = curr->data;
	
	// unlink this from the list

	if( prev != NULL ) {	// not the first element
		prev->next = curr->next;
	} else {			// first element
		q->head = curr->next;
	}

	if( curr->next == NULL ) {	// last element
		q->tail = prev;
	}

	// release the Kraken..., uh, qnode

	_qn_dealloc( curr );
	
	return( result );
}

/*
** _q_remove_by_data(que,data)
**
** remove the element identified by the specified data value from the queue
**
** returns the data field from the entry, or NULL
*/

void *_q_remove_by_data( queue_t *q, void *data ) {
	qnode_t *prev, *curr;
	void *result;
	
	// sanity check!

	if( q == NULL ) {
		_kpanic( "_q_remove_by_data", "NULL queue parameter" );
	}
	
	// is there anything to remove?

	if( q->size < 1 ) {
		return( NULL );
	}
	
	// non-empty queue; locate the specified element

#ifdef DEBUG
	c_printf( "*** _q_remove_by_data: scanning %d for %08x\n",
		  q->size, data );
#endif

	prev = NULL;
	curr = q->head;
	while( curr != NULL && curr->data != data ) {
#ifdef DEBUG
		c_printf( " skip %08x", curr->data );
#endif
		prev = curr;
		curr = curr->next;
	}
#ifdef DEBUG
	c_putchar( '\n' );
#endif

	// did we find it?

	if( curr == NULL ) {
		return( NULL );
	}

	// yes - copy out the data value

	result = curr->data;
	
	// unlink this from the list

	if( prev != NULL ) {	// not the first element
		prev->next = curr->next;
	} else {			// first element
		q->head = curr->next;
	}

	if( curr->next == NULL ) {	// last element
		q->tail = prev;
	}

	// release the Kraken..., uh, qnode

	_qn_dealloc( curr );
	
	return( result );
}

/*
** _q_peek(que)
**
** peek at the data of the first element in the queue
**
** returns the data field, or NULL
*/

void *_q_peek( queue_t *q ) {

	// sanity check

	if( q == NULL ) {
		_kpanic( "_q_peek", "NULL queue parameter" );
	}
	
	// can't peek if nothing is there

	if( q->size == 0 ) {
		return( NULL );
	}
	
	// return the data field

	return( q->head->data );
}

/*
** _q_kpeek(que)
**
** peek at the key of the first element in the queue
**
** returns the key, or NULL
*/

void *_q_kpeek( queue_t *q ) {

	// sanity check

	if( q == NULL ) {
		_kpanic( "_q_kpeek", "NULL queue parameter" );
	}
	
	// can't peek if nothing is there

	if( q->size == 0 ) {
		return( NULL );
	}
	
	// return the key field

	return( q->head->key );
}

/*
** _q_dump(which,queue)
**
** dump the contents of the specified queue to the console
*/

void _q_dump( char *which, queue_t *q ) {
	qnode_t *tmp;
	int i;

	c_printf( "%s: ", which );
	if( q == NULL ) {
		c_puts( "NULL???\n" );
		return;
	}

	c_printf( "head %08x tail %08x comp %08x (%d items)\n",
		  (uint32_t) q->head, (uint32_t) q->tail,
		  (uint32_t) q->cmp, q->size );
	
	if( q->size > 0 ) {
		c_puts( " data: " );
		i = 0;
		for( tmp = q->head; tmp != NULL; tmp = tmp->next ) {
			c_printf( " [%x,%x]", (uint32_t) tmp->key,
				(uint32_t) tmp->data );
			if( ++i > 10 ) break;
		}
		if( tmp != NULL ) {
			c_puts( " ..." );
		}
		c_puts( "\n" );
	}

}
E 1
