#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_





typedef struct PALETTE PALETTE;
typedef struct BITMAP BITMAP;

typedef struct GFX_CONTEXT {
    BITMAP * backbuffer;
    PALETTE * palette;
} GFX_CONTEXT;


//checkout klibc for blit and clearscreen ect

//do bitmap structs with attached data such as size ect also for screen
//blit buffer after vsync
//blit buffer after vsync

//use subatomic screen font for 

//text mode is 25 lines down

//guard if's to check if values are valid


//can i save the bios interupt 10h and go in and out ie switching programs

BITMAP create_bitmap(int width, int height, uint8_t transparent_color, uint8_t *data);


void putpixel(BITMAP *dest, int x, int y, uint8_t color);

void cleartocolor(BITMAP *dest, uint8_t color);

void drawline(BITMAP *dest, int x0, int y0, int x1, int y1, uint8_t color);

void blit(BITMAP *dest, BITMAP *src, int dest_x, int dest_y);

void blit_ex(BITMAP *dest, BITMAP *src, int dest_x, int dest_y, int src_x, int src_y, int width, int height);

uint8_t get_trans(BITMAP *bmp);

void text(BITMAP * dest, char * str, int x, int y, int centered);
void text_ex(BITMAP * dest, BITMAP * font_image, char * str, int x, int y, int centered);


#endif