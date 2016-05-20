#
# SCCS ID:	%W%	%G%
#
# Makefile to control the compiling, assembling and linking of
# standalone programs in the DSL.  Used for both individual
# interrupt handling assignments and the SP baseline OS (with
# appropriate tweaking).
#

#
# Lists of files
#
SYS_C_SRC = clock.c klibc.c process.c queue.c scheduler.c sio.c \
	stack.c syscall.c system.c graphics.c kgraphics.c images.c \
	diskdriver.c network.c netdrv.c

SYS_C_OBJ = clock.o klibc.o process.o queue.o scheduler.o sio.o \
	stack.o syscall.o system.o graphics.o kgraphics.o images.o \
	diskdriver.o network.o netdrv.o

SYS_S_SRC = klibs.S

SYS_S_OBJ = klibs.o

SYS_SRCS = $(SYS_S_SRC) $(SYS_S_SRC)
SYS_OBJS = $(SYS_S_OBJ) $(SYS_C_OBJ)

USR_C_SRC = ulibc.c user.c netconv.c network_test.c ip.c udp.c
USR_C_OBJ = ulibc.o user.o netconv.o network_test.o ip.o udp.o

USR_S_SRC = ulibs.S

USR_S_OBJ = ulibs.o

USR_SRCS = $(USR_S_SRC) $(USR_C_SRC)
USR_OBJS = $(USR_S_OBJ) $(USR_C_OBJ)

#
# User compilation/assembly definable options
#
#	ISR_DEBUGGING_CODE	include context restore debugging code
#	CLEAR_BSS_SEGMENT	include code to clear all BSS space
#	SP_OS_CONFIG		enable SP OS-specific startup variations
#	REPORT_MYSTERY_INTS	print a message on interrupt 0x27
#	DUMP_QUEUES		clock ISR dumps queues every 10 seconds
#	DEBUG			turns on debugging trace output
#
USER_OPTIONS = -DCLEAR_BSS_SEGMENT -DISR_DEBUGGING_CODE -DSP_OS_CONFIG

#
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!!!
#
# Compilation/assembly control
#

#
# We only want to include from the current directory and ~wrc/include
#
INCLUDES = -I. -Iinclude

#
# Compilation/assembly/linking commands and options
#
CPP = cpp
# CPPFLAGS = $(USER_OPTIONS) -nostdinc -I- $(INCLUDES)
CPPFLAGS = $(USER_OPTIONS) -nostdinc $(INCLUDES)

CC = gcc
CFLAGS = -m32 -std=c99 -fno-stack-protector -fno-builtin -Wall -Wstrict-prototypes $(CPPFLAGS)

AS = as
ASFLAGS = --32

LD = ld
LDFLAGS = -melf_i386

#
# Transformation rules - these ensure that all compilation
# flags that are necessary are specified
#
# Note use of 'cpp' to convert .S files to temporary .s files: this allows
# use of #include/#define/#ifdef statements. However, the line numbers of
# error messages reflect the .s file rather than the original .S file. 
# (If the .s file already exists before a .S file is assembled, then
# the temporary .s file is not deleted.  This is useful for figuring
# out the line numbers of error messages, but take care not to accidentally
# start fixing things by editing the .s file.)
#

.SUFFIXES:	.S .b

.c.s:
	$(CC) $(CFLAGS) -S $*.c

.S.s:
	$(CPP) $(CPPFLAGS) -o $*.s $*.S

.S.o:
	$(CPP) $(CPPFLAGS) -o $*.s $*.S
	$(AS) $(ASFLAGS) -o $*.o $*.s -a=$*.lst
	$(RM) -f $*.s

.s.b:
	$(AS) $(ASFLAGS) -o $*.o $*.s -a=$*.lst
	$(LD) $(LDFLAGS) -Ttext 0x0 -s --oformat binary -e begtext -o $*.b $*.o

.c.o:
	$(CC) $(CFLAGS) -c $*.c

# Framework files

# Binary/source file for system bootstrap code

BOOT_OBJ = bootstrap.b
BOOT_SRC = bootstrap.S

# Assembly language object/source files

FMK_S_OBJ = startup.o isr_stubs.o
FMK_S_SRC = startup.S isr_stubs.S

# C object/source files

FMK_C_OBJ = c_io.o support.o
FMK_C_SRC = c_io.c support.c

FMK_OBJS = $(FMK_S_OBJ) $(FMK_C_OBJ)
FMK_SRCS = $(FMK_S_SRC) $(FMK_C_SRC)

# System file collections

OBJECTS = $(FMK_OBJS) $(SYS_OBJS) $(USR_OBJS)
SOURCES = $(FMK_SRCS) $(SYS_SRCS) $(USR_SRCS)

#
# Targets for remaking bootable image of the program
#
# Default target:  usb.image
#

usb.image: bootstrap.b prog.b prog.nl BuildImage #prog.dis 
	./BuildImage -d usb -o usb.image -b bootstrap.b prog.b 0x10000

floppy.image: bootstrap.b prog.b prog.nl BuildImage #prog.dis 
	./BuildImage -d floppy -o floppy.image -b bootstrap.b prog.b 0x10000

prog.out: $(OBJECTS)
	$(LD) $(LDFLAGS) -o prog.out $(OBJECTS)

prog.o:	$(OBJECTS)
	$(LD) $(LDFLAGS) -o prog.o -Ttext 0x10000 $(OBJECTS)

prog.b:	prog.o
	$(LD) $(LDFLAGS) -o prog.b -s --oformat binary -Ttext 0x10000 prog.o

#
# Targets for copying bootable image onto boot devices
#

floppy:	floppy.image
	dd if=floppy.image of=/dev/fd0

usb:	usb.image
	/usr/local/dcs/bin/dcopy usb.image

#
# Special rule for creating the modification and offset programs
#
# These are required because we don't want to use the same options
# as for the standalone binaries.
#

BuildImage:	BuildImage.c
	$(CC) -o BuildImage BuildImage.c

Offsets:	Offsets.c
	$(CC) $(INCLUDES) -o Offsets Offsets.c

#
# Clean out this directory
#

clean:
	rm -f *.nl *.na *.lst *.b *.o *.image *.dis BuildImage Offsets

#
# Create a printable namelist from the prog.o file
#

prog.nl: prog.o
	nm -Bng prog.o | pr -w80 -3 > prog.nl
	nm -Bn  prog.o | pr -w80 -3 > prog.na

#
# Generate a disassembly
#

prog.dis: prog.o
	dis prog.o > prog.dis

#
#       makedepend is a program which creates dependency lists by
#       looking at the #include lines in the source files
#

depend:
	makedepend $(INCLUDES) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

startup.o: bootstrap.h
isr_stubs.o: bootstrap.h
c_io.o: c_io.h startup.h support.h include
support.o: startup.h support.h c_io.h include
support.o: bootstrap.h
ulibs.o: syscall.h common.h
ulibc.o: common.h ulib.h process.h stack.h queue.h
user.o: common.h user.h
