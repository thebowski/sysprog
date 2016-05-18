#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_
#include "startup.h"
#include "support.h"
#include "c_io.h"
#include "common.h"
#include "klib.h"

#define BASE 0x01f0
#define CONTROL 0x03f6
#define DATA 0x00
#define ERROR 0x01
#define SECTOR_COUNT 0x02
#define LBA_LO 0x03
#define LBA_MID 0x04
#define LBA_HI 0x05
#define DEVICE 0x06
#define COMMAND 0x07
#define STATUS 0x07

uint16_t disk_buffer[256];

int32_t disk_demo( void );
void disk_init( void );
void clear_filetable( void );
void read_filename( char *filename, uint8_t data[], uint16_t size );
void write_filename( char *filename, uint8_t data[], uint16_t size );
void write_sector( uint8_t data[], uint32_t lba );
void read_sector( uint8_t data[], uint32_t lba );
void print_data( uint8_t data[], int start, int end );
void identify( uint8_t data[] );
void print_identify( uint8_t data[] );
int strcmp( char *s1, char *s2 );

static void __diskdriver_06_isr( int vector, int code );
static void __diskdriver_0d_isr( int vector, int code );
static void __diskdriver_2e_isr( int vector, int code );

#endif

