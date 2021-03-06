h55932
s 00100/00000/00000
d D 1.1 16/04/22 16:40:46 wrc 1 0
c date and time created 16/04/22 16:40:46 by wrc
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
** File:	user.h
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	control of user-level routines
*/

#ifndef _USER_H_
#define _USER_H_

/*
** General (C and/or assembly) definitions
*/

// delay loop counts

#define	DELAY_LONG	100000000
#define	DELAY_STD	  2500000
#define	DELAY_ALT	  4500000

#ifndef __SP_ASM__

/*
** Start of C-only definitions
*/

/*
** User process controls.
**
** The comment field of these definitions contains a list of the
** system calls this user process uses.
**
** To spawn a specific user process from the initial process,
** uncomment its entry in this list.
*/

//	user	          baseline system calls in use
//		   exit rdch wrts fork exec wait kill slep gpid gppd gtim
#define	SPAWN_A	//  X    .    X    .    .    .    .    .    .    .    .
#define	SPAWN_B	//  X    .    X    .    .    .    .    .    .    .    .
#define	SPAWN_C	//  X    .    X    .    .    .    .    .    .    .    .
#define	SPAWN_D	//  X    .    X    X    X    .    .    .    .    .    .
#define	SPAWN_E	//  X    .    X    .    .    .    .    X    X    .    .
#define	SPAWN_F	//  X    .    X    .    .    .    .    X    X    .    .
#define	SPAWN_G	//  X    .    X    .    .    .    .    X    X    .    .
#define	SPAWN_H	//  .    .    X    .    .    .    .    .    .    .    .
// skipping user I
#define	SPAWN_J	//  X    .    X    X    X    .    .    .    .    .    .
#define	SPAWN_K	//  X    .    X    X    X    .    .    X    .    .    .
#define	SPAWN_L	//  X    .    X    X    X    .    .    X    .    .    X
#define	SPAWN_M	//  X    .    X    X    X    .    .    .    .    .    .
#define	SPAWN_N	//  X    .    X    X    X    .    .    .    .    .    .
// skipping user O
#define	SPAWN_P	//  X    .    X    .    .    .    .    X    .    .    X
#define	SPAWN_Q	//  X    .    X    .    .    .    .    .    .    .    .
		// user Q also makes a bogus system call
#define	SPAWN_R	//  X    X    X    .    .    .    .    X    .    .    .
#define	SPAWN_S	//  X    .    X    .    .    .    .    X    .    .    .
#define	SPAWN_T	//  X    .    X    X    X    X    .    X    .    .    .
#define	SPAWN_U	//  X    .    X    X    X    X    X    X    .    .    .
// no user V

/*
** Users W-Z are spawned from other processes; they
** should never be spawned directly by init().
*/

// user W:      //  X    .    X    .    .    .    .    X    X    .    .
// user X:      //  X    .    X    .    .    .    .    .    X    .    .
		// user X exits with a non-zero status
// user Y:      //  X    .    X    .    .    .    .    X    X    .    .
// user Z:      //  X    .    X    .    .    .    .    .    X    X    .


/*
** Prototypes for externally-visible routines
*/

/*
** init - initial user process
**
** after spawning the other user processes, loops forever calling wait()
*/

int32_t init( void );

/*
** idle - what to run when there's nothing else to run
*/

int32_t idle( void );

#endif

#endif
E 1
