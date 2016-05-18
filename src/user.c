/*
** SCCS ID:	@(#)user.c	1.1	4/22/16
**
** File:	user.c
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	User routines.
*/

#include "common.h"
#include "diskdriver.h"
#include "user.h"

/*
** USER PROCESSES
**
** Each is designed to test some facility of the OS:
**
**	User(s)		Tests/Features
**	=======		===============================================
**	A, B, C		Basic operation
**	D		Spawns Z and exits
**	E, F, G		Sleep for different lengths of time
**	H		Doesn't call exit()
**	J		Tries to spawn 2*N_PROCS copies of Y
**	K		Spawns several copies of X, sleeping between
**	L		Spawns several copies of X, preempting between
**	M		Spawns W three times, reporting PIDs
**	N		Like M, but spawns X and W
**	P		Iterates three times, sleeping & printing system time
**	Q		Tries to execute a bogus system call (bad code)
**	R		Reading and writing three characters
**	S		Loops forever, sleeping 30 seconds at a time
**	T		Loops, spawning W and X procs then waiting
**	U		Spawns W three times, sleeps, then kills them
**
**	W, X, Y, Z	Print characters (spawned by other processes)
**
** Output from user processes is always alphabetic.  Uppercase 
** characters are "expected" output; lowercase are "erroneous"
** output.
**
** More specific information about each user process can be found in
** the header comment for that function (below).
**
** To spawn a specific user process, uncomment its SPAWN_x
** definition in the user.h header file.
*/

/*
** Prototypes for all one-letter user main routines (even
** ones that may not exist, for completeness)
*/

int32_t user_a( void ); int32_t user_b( void ); int32_t user_c( void );
int32_t user_d( void ); int32_t user_e( void ); int32_t user_f( void );
int32_t user_g( void ); int32_t user_h( void ); int32_t user_i( void );
int32_t user_j( void ); int32_t user_k( void ); int32_t user_l( void );
int32_t user_m( void ); int32_t user_n( void ); int32_t user_o( void );
int32_t user_p( void ); int32_t user_q( void ); int32_t user_r( void );
int32_t user_s( void ); int32_t user_t( void ); int32_t user_u( void );
int32_t user_v( void ); int32_t user_w( void ); int32_t user_x( void );
int32_t user_y( void ); int32_t user_z( void );

/*
** Users A, B, and C are identical, except for the character they
** print out via swrites().  Each prints its ID, then loops 30
** times delaying and printing, before exiting.  They also verify
** the status return from swrites().
*/

int32_t user_me( void) {
    return disk_demo();
}

int32_t user_a( void ) {
	int i, j;
	int32_t status;
	char buf[12];

	status = swrites( "A", 1 );
	if( status != SUCCESS ) {
		cwrites( "User A, write 1 status ", 23 );
		i = cvt_dec( buf, status );
		cwrites( buf, i );
		cwrites( "\n", 1 );
	}
	for( i = 0; i < 10; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		status = swrites( "A", 1 );
		if( status != SUCCESS ) {
			cwrites( "User A, write 2 status ", 23 );
			i = cvt_dec( buf, status );
			cwrites( buf, i );
			cwrites( "\n", 1 );
		}
	}

	exit( EXIT_SUCCESS );

	status = swrites( "a", 1 );	/* shouldn't happen! */
	if( status != SUCCESS ) {
		cwrites( "User A, write 3 status ", 23 );
		i = cvt_dec( buf, status );
		cwrites( buf, i );
		cwrites( "\n", 1 );
	}

	return( 0 );  // shut the compiler up!

}

int32_t user_b( void ) {
	int i, j;
	int32_t status;
	char buf[12];

	status = swrites( "B", 1 );
	if( status != SUCCESS ) {
		cwrites( "User B, write 1 status ", 23 );
		i = cvt_dec( buf, status );
		cwrites( buf, i );
		cwrites( "\n", 1 );
	}
	for( i = 0; i < 30; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		status = swrites( "B", 1 );
		if( status != SUCCESS ) {
			cwrites( "User B, write 2 status ", 23 );
			i = cvt_dec( buf, status );
			cwrites( buf, i );
			cwrites( "\n", 1 );
		}
	}

	exit( EXIT_SUCCESS );

	status = swrites( "b", 1 );	/* shouldn't happen! */
	if( status != SUCCESS ) {
		cwrites( "User B, write 3 status ", 23 );
		i = cvt_dec( buf, status );
		cwrites( buf, i );
		cwrites( "\n", 1 );
	}
	return( 0 );  // shut the compiler up!

}

int32_t user_c( void ) {
	int i, j;
	int32_t status;
	char buf[12];

	status = swrites( "C", 1 );
	if( status != SUCCESS ) {
		cwrites( "User C, write 1 status ", 23 );
		i = cvt_dec( buf, status );
		cwrites( buf, i );
		cwrites( "\n", 1 );
	}
	for( i = 0; i < 30; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		status = swrites( "C", 1 );
		if( status != SUCCESS ) {
			cwrites( "User C, write 2 status ", 23 );
			i = cvt_dec( buf, status );
			cwrites( buf, i );
			cwrites( "\n", 1 );
		}
	}

	exit( EXIT_SUCCESS );

	status = swrites( "c", 1 );	/* shouldn't happen! */
	if( status != SUCCESS ) {
		cwrites( "User C, write 3 status ", 23 );
		i = cvt_dec( buf, status );
		cwrites( buf, i );
		cwrites( "\n", 1 );
	}
	return( 0 );  // shut the compiler up!

}

/*
** User D spawns user Z, then exits before it can terminate.
*/

int32_t user_d( void ) {
	int pid;

	swrites( "D", 1 );

	pid = fork();
	if( pid < 0 ) {
		cwrite( "User D fork() failed\n" );
	} else if( pid == 0 ) {
		exec( user_z );
		cwrite( "User D, exec(Z) failed\n" );
		exit( EXIT_FAILURE );
	}

	swrites( "D", 1 );

	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}


/*
** Users E, F, and G test the sleep facility.
**
** User E sleeps for 10 seconds at a time.
*/

int32_t user_e( void ) {
	int i;
	int32_t pid;
	char buf[12];

	pid = getpid();
	cwrites( "User E (", 8 );
	i = cvt_dec( buf, pid );
	cwrites( buf, i );
	cwrites( ") running\n", 10 );
	swrites( "E", 1 );
	for( i = 0; i < 5 ; ++i ) {
		sleep( 10 );
		swrites( "E", 1 );
	}

	exit( EXIT_SUCCESS );
	return( 0 );  // shut the compiler up!

}


/*
** User F sleeps for 5 seconds at a time.
*/

int32_t user_f( void ) {
	int i;
	int32_t pid;
	char buf[12];

	pid = getpid();
	cwrites( "User F (", 8 );
	i = cvt_dec( buf, pid );
	cwrites( buf, i );
	cwrites( ") running\n", 10 );
	swrites( "F", 1 );
	for( i = 0; i < 5 ; ++i ) {
		sleep( 5 );
		swrites( "F", 1 );
	}

	exit( EXIT_SUCCESS );
	return( 0 );  // shut the compiler up!

}


/*
** User G sleeps for 15 seconds at a time.
*/

int32_t user_g( void ) {
	int i;
	int32_t pid;
	char buf[12];

	pid = getpid();
	cwrites( "User G (", 8 );
	i = cvt_dec( buf, pid );
	cwrites( buf, i );
	cwrites( ") running\n", 10 );
	swrites( "G", 1 );
	for( i = 0; i < 5 ; ++i ) {
		sleep( 15 );
		swrites( "G", 1 );
	}

	exit( EXIT_SUCCESS );
	return( 0 );  // shut the compiler up!

}


/*
** User H is like A-C except it only loops 5 times and doesn't
** call exit().
*/

int32_t user_h( void ) {
	int i, j;

	swrites( "H", 1 );
	for( i = 0; i < 5; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		swrites( "H", 1 );
	}

	cwrite( "User H returning without exiting!\n" );

	return( 0 );  // shut the compiler up!

}


/*
** User J tries to spawn 2*N_PROCS copies of user_y.
*/

int32_t user_j( void ) {
	int i;
	int pid;

	swrites( "J", 1 );

	for( i = 0; i < N_PROCS * 2 ; ++i ) {
		pid = fork();
		if( pid < 0 ) {
			cwrite( "User J fork() failed\n" );
		} else if( pid == 0 ) {
			exec( user_y );
			cwrite( "User J, exec(Y) failed\n" );
			exit( EXIT_FAILURE );
		} else {
			swrites( "J", 1 );
		}
	}

	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}

/*
** User K prints, goes into a loop which runs three times, and exits.
** In the loop, it does a spawn of user_x, sleeps 30 seconds, and prints.
*/

int32_t user_k( void ) {
	int i;
	int pid;

	swrites( "K", 1 );

	for( i = 0; i < 3 ; ++i ) {
		pid = fork();
		if( pid < 0 ) {
			cwrite( "User K fork() failed\n" );
		} else if( pid == 0 ) {
			exec( user_x );
			cwrite( "User K, exec(X) failed\n" );
			exit( EXIT_FAILURE );
		} else {
			sleep( 30 );
			swrites( "K", 1 );
		}
	}

	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}


/*
** User L is like user K, except that it prints times and doesn't sleep
** each time, it just preempts itself.
*/

int32_t user_l( void ) {
	int i;
	int pid;
	uint32_t time;
	char buf[8];

	time = gettime();
	cwrite( "User L running, initial time " );
	i = cvt_hex( buf, time );
	cwrites( buf, i );
	cwrites( "\n", 1 );

	swrites( "L", 1 );

	for( i = 0; i < 3 ; ++i ) {
		pid = fork();
		if( pid < 0 ) {
			cwrite( "User L fork() failed\n" );
		} else if( pid == 0 ) {
			exec( user_x );
			cwrite( "User L, exec(X) failed\n" );
			exit( EXIT_FAILURE );
		} else {
			// yield, but don't sleep
			sleep( 0 );
			swrites( "L", 1 );
		}
	}

	time = gettime();
	cwrite( "User L exiting, time " );
	i = cvt_hex( buf, time );
	cwrites( buf, i );
	cwrites( "\n", 1 );

	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}


/*
** User M iterates spawns three copies of user W,
** reporting their PIDs.
*/

int32_t user_m( void ) {
	int i;
	int pid;
	char buf[12];

	swrites( "M", 1 );

	for( i = 0; i < 3; ++i ) {
		pid = fork();
		if( pid < 0 ) {
			cwrite( "User M fork() failed\n" );
		} else if( pid == 0 ) {
			exec( user_w );
			cwrite( "User M, exec(W) failed\n" );
			exit( EXIT_FAILURE );
		} else {
			cwrite( "User M spawned W, PID " );
			i = cvt_dec( buf, pid );
			cwrites( buf, i );
			cwrites( "\n", 1 );
			swrites( "M", 1 );
		}
	}

	exit( EXIT_SUCCESS );
	return( 0 );  // shut the compiler up!

}


/*
** User N is like user M, except that it spawns user W and user Z
*/

int32_t user_n( void ) {
	int i;
	int pid;
	char buf[12];

	swrites( "N", 1 );

	for( i = 0; i < 3; ++i ) {
		pid = fork();
		pid = fork();
		if( pid < 0 ) {
			cwrite( "User N fork() failed\n" );
		} else if( pid == 0 ) {
			exec( user_w );
			cwrite( "User N, exec(W) failed\n" );
			exit( EXIT_FAILURE );
		} else {
			cwrite( "User N spawned W, PID " );
			i = cvt_dec( buf, pid );
			cwrites( buf, i );
			cwrites( "\n", 1 );
		}

		pid = fork();
		pid = fork();
		if( pid < 0 ) {
			cwrite( "User N fork() failed\n" );
		} else if( pid == 0 ) {
			exec( user_z );
			cwrite( "User N, exec(Z) failed\n" );
			exit( EXIT_FAILURE );
		} else {
			cwrite( "User N spawned Z, PID " );
			i = cvt_dec( buf, pid );
			cwrites( buf, i );
			cwrites( "\n", 1 );
		}
		swrites( "N", 1 );
	}

	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}


/*
** User P iterates three times.  Each iteration sleeps for two seconds,
** then gets and prints the system time.
*/

int32_t user_p( void ) {
	uint32_t time;
	int i;
	char buf[8];

	cwrite( "User P running, start at " );
	time = gettime();
	i = cvt_hex( buf, time );
	cwrites( buf, i );
	cwrites( "\n", 1 );

	swrites( "P", 1 );

	for( i = 0; i < 3; ++i ) {
		sleep( 2 );
		time = gettime();
		cwrite( "User P reporting time " );
		i = cvt_hex( buf, time );
		cwrites( buf, i );
		cwrites( "\n", 1 );
		swrites( "P", 1 );
	}

	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}


/*
** User Q does a bogus system call
*/

int32_t user_q( void ) {

	swrites( "Q", 1 );
	bogus();
	cwrite( "User Q returned from bogus syscall!?!?!\n" );
	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}


/*
** User R loops 3 times reading/writing, then exits.
*/

int32_t user_r( void ) {
	int i, j;
	char ch = '&';
	char buf[12];

	sleep( 10 );
	for( i = 0; i < 3; ++i ) {
		do {
			swrites( "R", 1 );
			ch = sreadch();
			if( ch < 0 ) {
				cwrite( "User R, read returned " );
				j = cvt_dec( buf, ch );
				cwrites( buf, j );
				cwrites( "\n", 1 );
			} else if( ch == -1 ) {	/* wait a bit */
				sleep( 1 );
			}
		} while( ch < 0 );
		swrites( &ch, 1 );
	}

	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}


/*
** User S sleeps for 20 seconds at a time, and loops forever.
*/

int32_t user_s( void ) {

	swrites( "S", 1 );
	for(;;) {
		sleep( 20 );
		swrites( "S", 1 );
	}

	cwrite( "User S exiting!?!?!n" );
	swrites( "s", 1 );
	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}

/*
** User T iterates three times, spawning copies of user W; then it
** sleeps for eight seconds, and then waits for those processes.
*/

int32_t user_t( void ) {
	int i, j;
	int pid[3];
	int32_t status;
	char buf[12];


	for( i = 0; i < 3; ++i ) {
		pid[i] = fork();
		if( pid[i] < 0 ) {
			swrites( "t", 1 );
		} else if( pid[i] == 0 ) {
			exec( user_w );
			cwrite( "User T (" );
			j = cvt_dec( buf, getpid() );
			cwrites( buf, j );
			cwrite( " exec(W) failed\n" );
			exit( EXIT_FAILURE );
		} else {
			cwrite( "User T spawned W, PID " );
			j = cvt_dec( buf, pid[i] );
			cwrites( buf, j );
			cwrites( "\n", 1 );
			swrites( "T", 1 );
		}
	}

	sleep( 8 );

	// collect exit status information

	do {
		status = wait();
		if( status == E_NO_PROCS ) {
			break;
		}
		cwrite( "User T: child exit status " );
		j = cvt_dec( buf, status );
		cwrites( buf, j );
		cwrites( "\n", 1 );
	} while( 1 );

	exit( EXIT_SUCCESS );
	return( 0 );  // shut the compiler up!

}


/*
** User U is like user T, except that it kills any living children
** after sleeping.
*/

int32_t user_u( void ) {
	int i, j;
	int pid[3];
	int32_t status;
	char buf[12];


	for( i = 0; i < 3; ++i ) {
		pid[i] = fork();
		if( pid[i] < 0 ) {
			swrites( "u", 1 );
		} else if( pid[i] == 0 ) {
			exec( user_w );
			cwrite( "User U (" );
			j = cvt_dec( buf, getpid() );
			cwrites( buf, j );
			cwrite( " exec(W) failed\n" );
			exit( EXIT_FAILURE );
		} else {
			cwrite( "User U spawned W, PID " );
			j = cvt_dec( buf, pid[i] );
			cwrites( buf, j );
			cwrites( "\n", 1 );
			swrites( "U", 1 );
		}
	}

	sleep( 8 );

	for( i = 0; i < 3; ++i ) {
		if( pid[i] > 0 ) {
			status = kill( pid[i] );
			cwrite( "User U kill PID " );
			j = cvt_dec( buf, pid[i] );
			cwrites( buf, j );
			cwrite( " status " );
			j = cvt_dec( buf, status );
			cwrites( buf, j );
			cwrites( "\n", 1 );
		}
	}

	// collect exit status information

	do {
		status = wait();
		if( status == E_NO_PROCS ) {
			break;
		}
		cwrite( "User U: child exit status " );
		j = cvt_dec( buf, status );
		cwrites( buf, j );
		cwrites( "\n", 1 );
	} while( 1 );

	exit( EXIT_SUCCESS );
	return( 0 );  // shut the compiler up!

}


/*
** User W prints W characters 20 times, sleeping 3 seconds between.
*/

int32_t user_w( void ) {
	int i;
	int32_t pid;
	char buf[12];

	pid = getpid();
	cwrites( "User W (", 8 );
	i = cvt_dec( buf, pid );
	cwrites( buf, i );
	cwrites( ") running\n", 10 );

	for( i = 0; i < 20 ; ++i ) {
		swrites( "W", 1 );
		sleep( 3 );
	}

	cwrites( "User W (", 8 );
	cwrites( buf, i );
	cwrites( ") exiting\n", 10 );

	exit( EXIT_SUCCESS );
	return( 0 );  // shut the compiler up!

}


/*
** User X prints X characters 20 times.  It is spawned multiple
** times, prints its PID when started and exiting, and exits
** with a non-zero status
*/

int32_t user_x( void ) {
	int i, j;
	int32_t pid;
	char buf[12];

	pid = getpid();
	cwrites( "User X (", 8 );
	i = cvt_dec( buf, pid );
	cwrites( buf, i );
	cwrites( ") running\n", 10 );

	for( i = 0; i < 20 ; ++i ) {
		swrites( "X", 1 );
		for( j = 0; j < DELAY_STD; ++j )
			continue;
	}

	cwrites( "User X (", 8 );
	cwrites( buf, i );
	cwrites( ") exiting\n", 10 );
	exit( pid ? pid : -98765 );

	return( 0 );  // shut the compiler up!

}


/*
** User Y prints Y characters 10 times.
*/

int32_t user_y( void ) {
	int i, j;

	for( i = 0; i < 10 ; ++i ) {
		swrites( "Y", 1 );
		for( j = 0; j < DELAY_ALT; ++j )
			continue;
		sleep( 1 );
	}

	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

}


/*
** User Z prints Z characters 10 times.  Before it exits, its parent
** may have exited; it reports this so that we can verify reparenting.
*/

int32_t user_z( void ) {
	int i, j;
	int32_t pid, ppid;
	char buf[12];

	pid = getpid();
	ppid = getppid();

	cwrites( "User Z (", 8 );
	i = cvt_dec( buf, pid );
	cwrites( buf, i );
	cwrite( ") running, parent " );
	i = cvt_dec( buf, ppid );
	cwrites( buf, i );
	cwrites( "\n", 1 );

	for( i = 0; i < 10 ; ++i ) {
		swrites( "Z", 1 );
		for( j = 0; j < DELAY_STD; ++j )
			continue;
	}

	// get "new" parent PID
	ppid = getppid();

	cwrites( "User Z (", 8 );
	i = cvt_dec( buf, pid );
	cwrites( buf, i );
	cwrite( ") exiting, parent now " );
	i = cvt_dec( buf, ppid );
	cwrites( buf, i );
	cwrites( "\n", 1 );

	exit( EXIT_SUCCESS );

	return( 0 );  // shut the compiler up!

	return( 0 );  // shut the compiler up!

}


/*
** SYSTEM PROCESSES
*/

/*
** Idle process
*/

int32_t idle( void ) {
	int i;
	int32_t pid;
	uint32_t time;
	char buf[12];

	pid = getpid();
	time = gettime();
	cwrites( "Idle (", 6 );
	i = cvt_dec( buf, pid );
	cwrites( buf, i );
	cwrite( ") started @ " );
	i = cvt_hex( buf, time );
	cwrites( buf, i );
	cwrites( "\n", 1 );

	swrites( ".", 1 );

	for(;;) {
		for( i = 0; i < DELAY_LONG; ++i )
			continue;
		swrites( ".", 1 );
	}

	time = gettime();
	cwrite( "+++ Idle done @ " );
	i = cvt_hex( buf, time );
	cwrites( buf, i );
	cwrite( "!?!?!\n" );

	exit( EXIT_FAILURE );
	return( 0 );  // shut the compiler up!

}


/*
** Initial process; it starts the other top-level user processes.
*/

int32_t init( void ) {
	int pid, i;
	int32_t status;
	char buf[12];

	cwrites( "Init started\n", 13 );

	swrites( "$", 1 );

	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user ME failed\n" );
	} else if( pid == 0 ) {
		exec( user_me );
		cwrite( "init, exec() user ME failed\n" );
		exit( EXIT_FAILURE );
	}
	swrites( "/a/", 3 );

#ifdef SPAWN_A
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user A failed\n" );
	} else if( pid == 0 ) {
		exec( user_a );
		cwrite( "init, exec() user A failed\n" );
		exit( EXIT_FAILURE );
	}
	swrites( "/a/", 3 );
#endif

#ifdef SPAWN_B
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user B failed\n" );
	} else if( pid == 0 ) {
		exec( user_b );
		cwrite( "init, exec() user B failed\n" );
		exit( EXIT_FAILURE );
	}
	swrites( "/b/", 3 );
#endif

#ifdef SPAWN_C
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user C failed\n" );
	} else if( pid == 0 ) {
		exec( user_c );
		cwrite( "init, exec() user C failed\n" );
		exit( EXIT_FAILURE );
	}
	swrites( "/c/", 3 );
#endif

#ifdef SPAWN_D
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user D failed\n" );
	} else if( pid == 0 ) {
		exec( user_d );
		cwrite( "init, exec() user D failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_E
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user E failed\n" );
	} else if( pid == 0 ) {
		exec( user_e );
		cwrite( "init, exec() user E failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_F
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user F failed\n" );
	} else if( pid == 0 ) {
		exec( user_f );
		cwrite( "init, exec() user F failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_G
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user G failed\n" );
	} else if( pid == 0 ) {
		exec( user_g );
		cwrite( "init, exec() user G failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_H
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user H failed\n" );
	} else if( pid == 0 ) {
		exec( user_h );
		cwrite( "init, exec() user H failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_J
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user J failed\n" );
	} else if( pid == 0 ) {
		exec( user_j );
		cwrite( "init, exec() user J failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_K
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user K failed\n" );
	} else if( pid == 0 ) {
		exec( user_k );
		cwrite( "init, exec() user K failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_L
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user L failed\n" );
	} else if( pid == 0 ) {
		exec( user_l );
		cwrite( "init, exec() user L failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_M
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user M failed\n" );
	} else if( pid == 0 ) {
		exec( user_m );
		cwrite( "init, exec() user M failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_N
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user N failed\n" );
	} else if( pid == 0 ) {
		exec( user_n );
		cwrite( "init, exec() user N failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_P
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user P failed\n" );
	} else if( pid == 0 ) {
		exec( user_p );
		cwrite( "init, exec() user P failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_Q
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user Q failed\n" );
	} else if( pid == 0 ) {
		exec( user_q );
		cwrite( "init, exec() user Q failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_R
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user R failed\n" );
	} else if( pid == 0 ) {
		exec( user_r );
		cwrite( "init, exec() user R failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_S
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user S failed\n" );
	} else if( pid == 0 ) {
		exec( user_s );
		cwrite( "init, exec() user S failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_T
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user T failed\n" );
	} else if( pid == 0 ) {
		exec( user_t );
		cwrite( "init, exec() user T failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

#ifdef SPAWN_U
	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user U failed\n" );
	} else if( pid == 0 ) {
		exec( user_u );
		cwrite( "init, exec() user U failed\n" );
		exit( EXIT_FAILURE );
	}
#endif

	swrites( "!", 1 );

	/*
	** At this point, we go into an infinite loop waiting
	** for our children (direct, or inherited) to exit.
	*/

	for(;;) {
		status = wait();
		cwrite( "INIT: child exited, status " );
		i = cvt_dec( buf, status );
		cwrites( buf, i );
		cwrites( "\n", 1 );
	}

	/*
	** SHOULD NEVER REACH HERE
	*/

	cwrites( "*** INIT IS EXITING???\n", 23 );
	exit( EXIT_FAILURE );

	return( 0 );  // shut the compiler up!

}
