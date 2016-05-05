#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


#define VGA_START ((uint8_t*) 0x0A0000)
#define SCREEN_W 320
#define SCREEN_H 200
#define VGA_SIZE (SCREEN_W * SCREEN_H)

typedef struct BITMAP BITMAP;

extern BITMAP *screen;
extern BITMAP *backbuffer;

extern BITMAP *sprite;
extern BITMAP *sprite2;


//checkout klibc for blit and clearscreen ect

//do bitmap structs with attached data such as size ect also for screen
//blit buffer after vsync
//blit buffer after vsync

//use subatomic screen font for 

//text mode is 25 lines down

//guard if's to check if values are valid


//can i save the bios interupt 10h and go in and out ie switching programs


void putpixel(BITMAP *dest, int x, int y, uint8_t color);

void cleartocolor(BITMAP *dest, uint8_t color);

void drawline(BITMAP *dest, int x0, int y0, int x1, int y1, uint8_t color);

void blit(BITMAP *dest, BITMAP *src, int dest_x, int dest_y);

void blit_ex(BITMAP *dest, BITMAP *src, int dest_x, int dest_y, int src_x, int src_y, int width, int height);

void vsync(void);

uint8_t get_trans(BITMAP *bmp);

#endif