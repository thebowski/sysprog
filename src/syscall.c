/*
** SCCS ID:	@(#)syscall.c	1.1	4/22/16
**
** File:	syscall.c
**
** Author:	CSCI-452 class of 20155
**
** Contributor: Matthew Cheman mnc3139
**
** Description:	implementation of system call module
*/

#define    __SP_KERNEL__

#include "common.h"

#include <x86arch.h>
#include <uart.h>
#include "support.h"
#include "startup.h"

#include "syscall.h"
#include "scheduler.h"
#include "process.h"
#include "stack.h"
#include "clock.h"
#include "sio.h"
#include "kgraphics.h"

/*
** PRIVATE DEFINITIONS
*/

/*
** PRIVATE DATA TYPES
*/

/*
** PRIVATE GLOBAL VARIABLES
*/

// system call jump table
//
// initialized by _sys_init() to ensure that the
// code::function mappings are correct

static void (*_syscalls[N_SYSCALLS])(pcb_t *pcb);

/*
** PUBLIC GLOBAL VARIABLES
*/

queue_t _zombie;    // "zombie" processes
queue_t _waiting;    // "waiting for child" processes

/*
** PRIVATE FUNCTIONS
*/

/*
** _sys_isr(vector,code)
**
** Common handler for the system call module.  Selects
** the correct second-level routine to invoke based on
** the contents of EAX.
**
** The second-level routine is invoked with a pointer to
** the PCB for the process.  It is the responsibility of
** that routine to assign all return values for the call.
*/

static void _sys_isr(int vector, int code) {
    uint32_t syscode;

    // retrieve the syscall code

    syscode = REG(_current, eax);

    // verify that it's valid

    if (syscode >= N_SYSCALLS) {

        // nope!  fake it to be exit(EXIT_BAD_CODE)

        syscode = SYS_exit;
        ARG(_current, 1) = EXIT_BAD_CODE;

    }

    // invoke the appropriate handler

    _syscalls[syscode](_current);

    // tell the PIC we're done

    __outb(PIC_MASTER_CMD_PORT, PIC_EOI);
}

/*
** Second-level syscall handlers
**
** All have this prototype:
**
**      static void _sys_NAME( pcb_t * );
**
** Values being returned to the user are placed into the EAX
** field in the context save area of the provided PCB.
*/

/*
** _sys_getpid - retrieve the PID of the current process
**
** implements:  uint32_t getpid();
**
** returns:
**      the process' pid
*/

static void _sys_getpid(pcb_t *pcb) {
    RET(pcb) = pcb->pid;
}

/*
** _sys_getppid - retrieve the parent PID of the current process
**
** implements:  uint32_t getppid();
**
** returns:
**      the process' parent's pid
*/

static void _sys_getppid(pcb_t *pcb) {
    RET(pcb) = pcb->ppid;
}

/*
** _sys_gettime - retrieve the current system time
**
** implements:  uint32_t gettime();
**
** returns:
**      the current system time
*/

static void _sys_gettime(pcb_t *pcb) {
    RET(pcb) = _system_time;
}

/*
** _sys_readch - read a single character from an input channel
**
** implements:	int32_t readch( uint32_t chan );
**
** blocks the calling routine if there is no character to return
**
** returns:
**	the character
*/

static void _sys_readch(pcb_t *pcb) {
    int ch;
    int chan = ARG(pcb, 1);

    // try to get the next character

    switch (chan) {

        case CHAN_CONSOLE:
            // console input doesn't block
            if (c_input_queue() < 1) {
                RET(pcb) = E_NO_CHAR;
                return;
            }
            ch = c_getchar();
            break;

        case CHAN_SIO:
            // this may block
            ch = _sio_readc();
            break;

        default:
            // bad channel code!
            RET(pcb) = E_BAD_CHAN;
            return;
    }

    // if there was a character, return it to the process;
    // otherwise, block the process until one comes in

    if (ch >= 0) {

        RET(pcb) = (uint32_t) ch & 0xff;

    } else {

        // no character; put this process on the
        // serial i/o input queue

        _current->state = PS_BLOCKED;

        _q_insert(&_reading, (void *) _current,
                  (void *) _current->pid);

        // select a new current process

        _dispatch();

    }

}

/*
** _sys_writes - write a sized buffer to a channel
**
** implements:	status_t writes( uint32_t chan, char *buf, int size );
**
** returns:
**	status of the operation
*/

static void _sys_writes(pcb_t *pcb) {
    int chan = ARG(pcb, 1);
    char *buf = (char *) ARG(pcb, 2);
    int size = ARG(pcb, 3);

    // this is almost insanely simple, but it does separate
    // the low-level device access fromm the higher-level
    // syscall implementation

    switch (chan) {

        case CHAN_CONSOLE:
            for (int i = 0; i < size; ++i) {
                c_putchar(buf[i]);
            }
            RET(pcb) = E_SUCCESS;
            break;

        case CHAN_SIO:
            // much simpler!
            _sio_writes(buf, size);
            RET(pcb) = E_SUCCESS;
            break;

        default:
            RET(pcb) = E_BAD_CHAN;
            break;

    }

}

/*
** _sys_sleep - put the current process to sleep for some length of time
**
** implements:  void sleep(uint32_t seconds);
**
** If the sleep time (in seconds) is 0, just preempts the process; else,
** puts it onto the sleep queue for the specified length of time.
*/

static void _sys_sleep(pcb_t *pcb) {
    uint32_t sleeptime = ARG(pcb, 1);

    if (sleeptime == 0) {

        // for a value of 0, just preempt

        _sched(_current);

    } else {

        // else, set wakeup time and put it to sleep

        // turn sleep time into ticks

        sleeptime = SECONDS_TO_TICKS(sleeptime);

        _current->wakeup = _system_time + sleeptime;
        _current->state = PS_SLEEPING;
        _q_insert(&_sleeping, (void *) pcb, (void *) _current->wakeup);

    }

    // regardless, this process won't continue, so get a new one

    _dispatch();
}

/*
** _sys_fork - create a new process
**
** implements:  pid = fork();
**
** returns on success:
**      PID of new process (in parent)
**      0 via the info pointer (in child)
**
** returns on failure:
**	-1
*/

static void _sys_fork(pcb_t *pcb) {

    /*
    ** allocate a pcb
    ** allocate a stack
    ** duplicate current pcb & stack
    ** fix %ebp chain
    ** return 0 in child, pid in parent
    */

    pcb_t *new;
    stack_t *stack;

    // allocate a pcb

    new = _pcb_alloc();
    if (new == NULL) {
        RET(pcb) = E_NO_PROCS;
        return;
    }

    // allocate a stack

    stack = _stk_alloc();
    if (stack == NULL) {
        RET(pcb) = E_NO_PROCS;
        // don't forget to put the PCB back!
        _pcb_dealloc(new);
        return;
    }

#ifdef DEBUG
    c_printf( "*** _sys_fork\n" );
    _pcb_dump( "old pcb", pcb );
    _context_dump( "old context", pcb->context );
    __delay( 100 );
#endif

    // duplicate the exiting PCB and stack into the new one

    _kmemcpy((uint8_t *) new, (uint8_t *) pcb, sizeof(pcb_t));
    _kmemcpy((uint8_t *) stack, (uint8_t *) pcb->stack, sizeof(stack_t));

    // OK, we have the data structures; start to fill them in

    new->pid = _next_pid++;
    new->ppid = pcb->pid;
    new->stack = stack;

    /*
    ** We duplicated the original stack contents, which
    ** means that the context pointer and ESP and EBP values
    ** in the new stack are still pointing into the original
    ** stack.  We need to correct all of these.
    **
    ** We have to change EBP because that's how the compiled
    ** code for the user process accesses its local variables.
    ** If we didn't change this, as soon as the new process
    ** was dispatched, it would start to stomp on the local
    ** variables in the original process' stack.  We also
           ** have to fix the EBP chain in the child process.
    **
    ** None of this would be an issue if we were doing "real"
    ** virtual memory, as we would be talking about virtual
    ** addresses here rather than physical addresses, and all
    ** processes would share the same virtual address space
    ** layout.
    **
    ** First, determine the distance (in bytes) between the
    ** two stacks.  This is the adjustment value we must add
    ** to the three pointers to correct them.  Note that this
    ** distance may be positive or negative, depending on the
    ** relative placement of the two stacks in memory.
    */


    int stack_offset = ((void *) new->stack - (void *) pcb->stack);

    // add this distance to the context pointer

    new->context = (context_t *)
            ((void *) (new->context) + stack_offset);

    /*
    ** fix EBP and ESP
    **
    ** this is easy, because they're just uint32_t values in
    ** the context save area)
    **
    ** technically, we don't really need to do ESP, because
    ** it will be "restored" by the popa instruction, but
    ** since we're here....
    */

    REG(new, ebp) += stack_offset;
    REG(new, esp) += stack_offset;

    // now, set the return values

    RET(pcb) = new->pid;
    RET(new) = 0;

#ifdef DEBUG
    _pcb_dump( "new pcb", new );
    _context_dump( "new context", new->context );
    __delay( 300 );
#endif

    // schedule the child; let the parent continue

    _sched(new);
}

/*
** exec - replace a process with a different program
**
** usage:       exec(entry);
**
** returns:
**      does not return (if the attempt succeeds)
*/

void _sys_exec(pcb_t *pcb) {
    uint32_t entry;

    // retrieve the entry point

    entry = ARG(pcb, 1);

#ifdef DEBUG
    c_printf( "*** _sys_exec, new entry %08x\n", entry );
    _pcb_dump( "old pcb", pcb );
#endif

    // reset the stack and the process context

    pcb->context = _stk_setup(pcb->stack, entry);

#ifdef DEBUG
    _pcb_dump( "new pcb", pcb );
    _context_dump( "new context", pcb->context );
    __delay( 200 );
#endif

    // philosophical question:  let this process continue,
    // or schedule it?  we choose the former.
}

/*
** _sys_exit - terminate the calling process
**
** implements:  void exit(code);
**
** does not return
*/

static void _sys_exit(pcb_t *pcb) {

#ifdef DEBUG
    c_printf( "*** _sys_exit, PID %d STATUS %d\n", pcb->pid, ARG(pcb,1) );
#endif

    // mark this as gone, but not forgotten

    pcb->state = PS_ZOMBIE;

    // reparent all its children

    for (int i = 0; i < N_PCBS; ++i) {
        if (_pcbs[i].ppid == pcb->pid &&
            _pcbs[i].state >= PS_FIRST_LIVE) {
            _pcbs[i].ppid = PID_INIT;
        }
    }

    // see if our parent is waiting for us; either give it
    // our status and clean us up, or turn us into a zombie

    _zombify(pcb, ARG(pcb, 1));

    // pick a new current process

    _dispatch();
}

/*
** _sys_wait - wait for one of the children of a process
**
** implements:	uint32_t wait( void );
**
** returns:
**	the exit status of the child
**	an error code if there are no children in the system
*/

static void _sys_wait(pcb_t *pcb) {
    int i;
    pcb_t *c1, *c2;

    /*
    ** Locate a child process that is either on the zombie queue
    ** (c1) or still executing (c2).  Keep a pointer to its PCB
    ** so that we can collect its information.
    */

    c1 = c2 = NULL;
    for (i = 0; i < N_PCBS; ++i) {
        if (_pcbs[i].ppid == pcb->pid &&
            _pcbs[i].state >= PS_FIRST_LIVE) {
            if (_pcbs[i].state == PS_ZOMBIE) {
                c1 = &_pcbs[i];
            } else {
                c2 = &_pcbs[i];
            }
        }
    }

    // if we didn't find one, we're done

    if (c1 == NULL && c2 == NULL) {
        RET(pcb) = E_NO_PROCS;
        return;
    }

    // OK, we know we have at least one child out there

    // did we find a zombie?  if so, collect its information and return

    if (c1 != NULL) {
        void *tmp;

        // pull this process off the zombie queue

        tmp = _q_remove_by_data(&_zombie, c1);
        if (tmp != c1) {
            _kpanic("_sys_wait", "zombie queue remove failed");
        }

        // copy the requested data into the caller

        RET(pcb) = ARG(c1, 1);

        // clean up the child

        _pcb_cleanup(c1);

        // we're done

        return;
    }

    // no zombie child; verify that there's still an active child

    if (c2 == NULL) {
        // this shouldn't be able to happen
        _pcb_dump("waiting parent", pcb);
        _kpanic("_sys_wait", "parent has children, but none found");
    }

    // found an active child, so we'll block until it (or another) exits

    pcb->state = PS_WAITING;
    _q_insert(&_waiting, (void *) pcb, (void *) (pcb->pid));

    // we're blocked, so select a new current process

    _dispatch();
}

/*
** _sys_kill - terminate a process with extreme prejudice
**
** implements:  int32_t kill(pid);
**
** returns:
**      status of the termination attempt
*/

void _sys_kill(pcb_t *pcb) {
    uint32_t pid;
    pcb_t *victim;

    // find the victim

    pid = ARG(pcb, 1);
    victim = _pcb_find(pid);

    // if there wasn't one, report that to the caller

    if (victim == NULL) {
        RET(pcb) = E_NO_PROCS;
        return;
    }

    // we have a victim; figure out where it is

    switch (victim->state) {

        case PS_RUNNING:
            // it's us!!!  turn this into an exit() call
            ARG(pcb, 1) = EXIT_KILLED;
            _sys_exit(pcb);
            return;

        case PS_ZOMBIE:
            // the victim is already dead
            RET(pcb) = E_NO_PROCS;
            return;


        case PS_READY:
        case PS_SLEEPING:
        case PS_BLOCKED:
        case PS_WAITING:
            // let _zombify() handle it
            _zombify(victim, EXIT_KILLED);
            break;

        case PS_UNUSED:
        case PS_NEW:
            // should never happen!!!
            c_printf("*** _sys_kill, victim state %d\n",
                     victim->state);
            _kpanic("_sys_kill", "victim is unused or new???");

        default:
            // should never happen!!!
            c_printf("*** _sys_kill, victim state %d\n",
                     victim->state);
            _kpanic("_sys_kill", "bad victim state");
    }
}

/*
** _sys_getgfxcontext - gets a graphics context that the process can draw to, contains a palette and bitmap to draw to
**		will reset context if called again by same process. backbuffer will not be drawn to screen until user calls
 * 		the drawscreen() syscall
 *
** implements:  GFX_CONTEXT * _sys_getgfxcontext(void);
**
** returns:
**      graphics context which contains backbuffer BITMAP and PALETTE. Defined in graphics.h
*/
GFX_CONTEXT *_sys_getgfxcontext(pcb_t *pcb) {

    RET(pcb) = _kgfx_new_context(pcb->pid);
}

/*
** _sys_drawscreen - Draws the backbuffer of the process's graphics context to the screen
 * 		if it is the os' active context
**
** implements:  void drawscreen(void);
**
** returns:
**      nothing
*/
void _sys_drawscreen(pcb_t *pcb) {

    _kgfx_draw_screen(pcb->pid);
}

/*
** PUBLIC FUNCTIONS
*/

/*
** _sys_init()
**
** initialize the syscall module
*/

void _sys_init(void) {

    /*
    ** Set up the syscall jump table.  We do this here
    ** to ensure that the association between syscall
    ** code and function address is correct even if the
    ** codes change.
    */

    _syscalls[SYS_exit] = _sys_exit;
    _syscalls[SYS_readch] = _sys_readch;
    _syscalls[SYS_writes] = _sys_writes;
    _syscalls[SYS_fork] = _sys_fork;
    _syscalls[SYS_exec] = _sys_exec;
    _syscalls[SYS_wait] = _sys_wait;
    _syscalls[SYS_kill] = _sys_kill;
    _syscalls[SYS_sleep] = _sys_sleep;
    _syscalls[SYS_getpid] = _sys_getpid;
    _syscalls[SYS_getppid] = _sys_getppid;
    _syscalls[SYS_gettime] = _sys_gettime;
    _syscalls[SYS_getgfxcontext] = _sys_getgfxcontext;
    _syscalls[SYS_drawscreen] = _sys_drawscreen;

    // initialize the zombie and waiting queues

    _q_reset(&_zombie, NULL);
    _q_reset(&_waiting, NULL);

    // install the second-stage ISR

    __install_isr(INT_VEC_SYSCALL, _sys_isr);

    // MORE TO COME?

    c_puts(" SYSCALL");
}
