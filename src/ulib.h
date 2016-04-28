/*
** SCCS ID:	@(#)ulib.h	1.1	4/22/16
**
** File:	ulib.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	declarations for user-level library functions
*/

#ifndef _ULIB_H_
#define _ULIB_H_

#include "common.h"

/*
** General (C and/or assembly) definitions
*/

#ifndef __SP_ASM__

#include "process.h"

/*
** Start of C-only definitions
*/

/*
** Types
*/

/*
** Globals
*/

/*
** Prototypes
*/

/*
** readch - read one character from an input channel
**
** usage:	n = readch( chan );
**
** reads one character from 'chan' and returns it
**
** returns:
**      the character read, or an error code
*/

int32_t readch( uint32_t channel );

/*
** writes - write a buffer to an output channel
**
** usage:	n = writes( chan, buf, size );
**
** writes 'size' characters from 'buf' to 'chan'
**
** returns:
**      a status code
*/

int32_t writes( uint32_t chan, char *buf, int size );

/*
** fork - create a new process
**
** usage:	pid = fork();
**
** returns:
**	pid of the spawned process, or -1 on failure
*/

int32_t fork( void );

/*
** exec - replace this program with a different one
**
** usage:	pid = exec(entry);
**
** returns:
**	on success, does not return
*/

void exec( int32_t (*main)(void) );

/*
** exit - terminate the calling process
**
** usage:	exit();
**
** does not return
*/

void exit( uint32_t status );

/*
** wait - wait for a child process to terminate
**
** usage:	status = wait();
**
** if there are no children in the system, returns an error code
**
** if there are one or more children in the system and at least one has
** terminated but hasn't yet been cleaned up, cleans up that process and
** returns its information; otherwise, blocks until a child terminates
**
** returns:
**      the exit status of the child on success
**      an error code on failure
*/

int32_t wait( void );

/*
** sleep - put the current process to sleep for some length of time
**
** usage:	sleep(n);
**
** puts the process onto the sleep queue for the specified
** number of seconds
*/

void sleep( uint32_t seconds );

/*
** kill - terminate a process with extreme prejudice
**
** usage:       status = kill(pid);
**
** returns:
**      status of the termination attempt
*/

int32_t kill( uint32_t pid );

/*
** getpid - retrieve PID of this process
**
** usage:	n = getpid();
**
** returns:
**      the PID of this process
*/

uint32_t getpid( void );

/*
** getppid - retrieve PID of the parent of this process
**
** usage:	n = getppid();
**
** returns:
**      the PID of the parent of this process
*/

uint32_t getppid( void );

/*
** gettime - retrieve the current system time
**
** usage:	n = gettime();
**
** returns:
**      the current system time
*/

uint32_t gettime( void );

/*
** bogus - a bogus system call, for testing our syscall ISR
**
** usage:	bogus();
*/

void bogus( void );

/*
** strlen()
**
** return length of a NUL-terminated string
*/

uint32_t strlen( char *msg );

/*
** cvt_dec()
**
** convert a 32-bit integer value into a
** NUL-terminated character string
**
** returns the number of characters placed into the buffer
** (not including the NUL)
*/

int cvt_dec( char *buf, int32_t value );

/*
** cvt_hex()
**
** convert a 32-bit value into an (up to) 8-character
** NUL-terminated character string
**
** returns the number of characters placed into the buffer
** (not including the NUL)
*/

int cvt_hex( char *buf, uint32_t value );

/*
** do_exit()
**
** support function used as the "return to" address
** for user main() routines; it just calls exit(%eax)
*/

void do_exit( void );

/*
** creadch()
**
** read a single character from the console
*/

int32_t creadch( void );

/*
** sreadch()
**
** read a single character from the SIO
*/

int32_t sreadch( void );

/*
** cwrite()
**
** write a NUL-terminated buffer to the console
*/

int32_t cwrite( char *buf );

/*
** swrite()
**
** write a NUL-terminated buffer to the SIO
*/

int32_t swrite( char *buf );

/*
** cwrites()
**
** write a sized buffer to the console
*/

int32_t cwrites( char *buf, int size );

/*
** swrites()
**
** write a sized buffer to the SIO
*/

int32_t swrites( char *buf, int size );

/*
** prt_status - print a status return value to the console
**
** the "msg" argument is printed, followed by a hex representation
** of the "stat" argument
*/

void prt_status( char *msg, uint32_t stat );

#endif

#endif
