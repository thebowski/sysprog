/*
** SCCS ID:	@(#)system.c	1.1	4/22/16
**
** File:	system.c
**
** Author:	CSCI-452 class of 20155
**
** Contributor: Matthew Cheman mnc3139
**
** Description:	miscellaneous OS support functions
*/

#define    __SP_KERNEL__

#include "common.h"

#include "system.h"
#include "clock.h"
#include "process.h"
#include "bootstrap.h"
#include "syscall.h"
#include "sio.h"
#include "scheduler.h"
#include "diskdriver.h"

// need init() and idle() addresses
#include "user.h"

#include "kgraphics.h"

// need the exit() and do_exit() prototypes
#include "ulib.h"

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

/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

/*
** _zombify(pcb,status)
**
** turn a process into a zombie, or give its status to
** a waiting parent
*/

void _zombify(pcb_t *pcb, uint32_t status) {
    uint32_t ppid;
    pcb_t *parent;
    void *tmp;
    queue_t *which;

    // sanity check!

    if (pcb == NULL) {
        _kpanic("_zombify", "NULL pcb parameter");
    }

#ifdef DEBUG
    _pcb_dump( "*** _zombify: processing", pcb );
#endif

    // check the state of this process

    which = NULL;

    switch (pcb->state) {

        case PS_RUNNING:
            // need another current process
            _dispatch();
            break;

        case PS_ZOMBIE:
            // already done!
#ifdef DEBUG
            _pcb_dump( "*** _zombify: found it as a zombie", pcb );
#endif
            return;
            break;

        case PS_READY:
            // remove it from the ready queue
            which = &_ready;
            break;

        case PS_SLEEPING:
            // remove it from the sleep queue
            which = &_sleeping;
            break;

        case PS_BLOCKED:
            // remove it from the blocked queue
            which = &_reading;
            break;

        case PS_WAITING:
            // remove it from the waiting queue
            which = &_waiting;
            break;

        case PS_UNUSED:
        case PS_NEW:
            // should never happen!
            c_printf("*** _zombify, victim state %d\n",
                     pcb->state);
            _kpanic("_zombify", "victime is unused or new???");
            break;

        default:
            // should never happen!
            c_printf("*** _zombify, victim state %d\n",
                     pcb->state);
            _kpanic("_zombify", "bad victime state");
            break;

    }

    // if we need to deque it, do so

    if (which != NULL) {
        tmp = _q_remove_by_data(which, pcb);
        if (tmp != pcb) {
            _kpanic("_zombify", "remove from current que fail");
        }
    }

    // mark the process as no longer runnable

    pcb->state = PS_ZOMBIE;

    // find the parent

    ppid = pcb->ppid;
    parent = _pcb_find(ppid);

    // If we didn't find a parent, report the problem

    if (parent == NULL) {
        c_printf("*** _zombify: want parent %d for %d\n",
                 pcb->ppid, pcb->pid);
        _kpanic("_zombify", "no parent for zombied proc");
    }

    /*
    ** At this point, parent points to the parent's PCB, and ppid
    ** contains the parent's PID.
    **
    ** If the parent is on the wait() queue, we'll awaken it and give
    ** it this child's information.
    **
    ** Otherwise, we need to put this child on the zombie queue.
    */

#ifdef DEBUG
    _pcb_dump( "*** _zombify: parent is", parent );
#endif

    if (parent->state == PS_WAITING) {

        // look for the parent on the wait queue

        tmp = _q_remove_by_data(&_waiting, parent);

        // verify that we found it

        if (tmp != parent) {
            _pcb_dump("_zombify waiting parent", parent);
            _kpanic("_zombify", "parent wait deque failed");
        }

        // OK, we have the right one.
        // return the child's information to it.

        RET(parent) = status;

        // schedule the parent (who returns from wait())

        _sched(parent);

        // clean up the child process
        _pcb_cleanup(pcb);

    } else {

        // parent isn't currently waiting, so
        // place this child on the zombie queue
        //
        // we use its PID as the key in order to
        // facilitate locating it later

        _q_insert(&_zombie, (void *) pcb, (void *) (pcb->pid));

    }
}

/*
** _init - system initialization routine
**
** Called by the startup code immediately before returning into the
** first user process.
*/

void _init(void) {
    pcb_t *pcb;

    _kgfx_init();

    /*
    ** BOILERPLATE CODE - taken from basic framework
    **
    ** Initialize interrupt stuff.
    */

    __init_interrupts();    // IDT and PIC initialization

    /*
    ** Console I/O system.
    */

    c_io_init();
    c_setscroll(0, 7, 99, 99);
    c_puts_at(0, 6, "================================================================================");

    /*
    ** 20155-SPECIFIC CODE STARTS HERE
    */

    /*
    ** Initialize various OS modules
    **
    ** Note:  the clock, SIO, and syscall modules also install
    ** their ISRs.
    */

	c_puts( "System initialization starting\n" );
	c_puts( "------------------------------\n" );
	//__delay( 200 );  // about 5 seconds

    c_puts("Module init: ");

    _q_init();        // must be first
    _pcb_init();        // processes
    _stk_init();        // stacks
    _sched_init();        // scheduler
    _sio_init();        // serial i/o
    _sys_init();        // syscalls
    _clk_init();        // clock

    c_puts("\n");

    /*
    ** Create the initial system ESP
    **
    ** This will be the address of the next-to-last
    ** longword in the system stack.
    */

    _system_esp = ((uint32_t *) ((&_system_stack) + 1)) - 2;

    /*
    ** Create the initial process
    **
    ** Code largely stolen from _sys_fork() and _sys_exec(); if
    ** either or both of those routines change, SO MUST THIS!!!
    */

    // allocate a PCB and stack

    pcb = _pcb_alloc();
    if (pcb == NULL) {
        _kpanic("_init", "init() pcb alloc failed");
    }

    pcb->stack = _stk_alloc();
    if (pcb->stack == NULL) {
        _kpanic("_init", "init() stack alloc failed");
    }

    // initialize the stack with the standard context

    pcb->context = _stk_setup(pcb->stack, (uint32_t) init);
    if (pcb->context == NULL) {
        _kpanic("_init", "init() stack setup failed");
    }

    // set up various PCB fields

    pcb->pid = pcb->ppid = _next_pid++;    // next PID is initially 1

#ifdef DEBUG
    _pcb_dump( "init() pcb", pcb );
    _context_dump( "init() context", pcb->context );
    __delay( 300 );
#endif

    // remember it

    _init_pcb = pcb;

    // put it on the ready queue

    _sched(pcb);

    // next, the idle routine - allocate a PCB and stack

    pcb = _pcb_alloc();
    if (pcb == NULL) {
        _kpanic("_init", "idle() pcb alloc failed");
    }

    pcb->stack = _stk_alloc();
    if (pcb->stack == NULL) {
        _kpanic("_init", "idle() stack alloc failed");
    }

    // initialize the stack with the standard context

    pcb->context = _stk_setup(pcb->stack, (uint32_t) idle);
    if (pcb->context == NULL) {
        _kpanic("_init", "idle() stack setup failed");
    }

    // set up various PCB fields

    pcb->pid = _next_pid++;
    pcb->ppid = PID_INIT;    // parent should be init

#ifdef DEBUG
    _pcb_dump( "idle() pcb", pcb );
    _context_dump( "idle() context", pcb->context );
    __delay( 300 );
#endif

    // remember it

    _idle_pcb = pcb;

    // try scheduling it, just to be sure it isn't actually scheduled

    _sched(pcb);

    /*
    ** Turn on the SIO receiver (the transmitter will be turned
    ** on/off as characters are being sent)
    */

    _sio_enable(SIO_RX);

    // Init disk driver
    disk_init();

	// dispatch the first user process
    _dispatch();

    /*
    ** END OF 20155-SPECIFIC CODE
    **
    ** Finally, report that we're all done.
    */

    c_puts("System initialization complete.\n");
    c_puts("Preparing to dispatch init()...\n");
    c_puts("-------------------------------\n");

}
