#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


#define VGA_START 0x0A0000
#define SCREEN_W 320
#define SCREEN_H 200
#define BPP 1
#define VGA_END (VGA_START + SCREEN_W * SCREEN_H * BPP)

typedef struct _BITMAP BITMAP;

extern BITMAP * screen;
extern BITMAP * backbuffer;


//checkout klibc for blit and clearscreen ect

//do bitmap structs with attached data such as size ect also for screen
//blit buffer after vsync
//blit buffer after vsync

//use subatomic screen font for 

//text mode is 25 lines down

//guard if's to check if values are valid


//can i save the bios interupt 10h and go in and out ie switching programs

void putpixel(BITMAP * dest, int x, int y, uint8_t color);

void cleartocolor(BITMAP * dest, uint8_t color);

void drawline(BITMAP * dest, int x0, int y0, int x1, int y1, uint8_t color);

void blit(BITMAP * dest, BITMAP * src, int dest_x, int dest_y);

void vsync(void);

#endif