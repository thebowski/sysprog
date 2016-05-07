/*
** File:	kgraphics.h
**
** Author:	Matthew Cheman mnc3139
**
** Contributor:
**
** Description:	?
*/

#ifndef _KGRAPHICS_H_
#define _KGRAPHICS_H_

#include "common.h"
#include "graphics.h"

/*
** General (C and/or assembly) definitions
*/

#define VGA_START 0x0A0000
#define SCREEN_W 320
#define SCREEN_H 200
#define VGA_SIZE (SCREEN_W * SCREEN_H)

#define PALETTE_SIZE (256 * 3)
#define VGA_PORT_PALETTE_INDEX 0x3C8
#define VGA_PORT_PALETTE_COLOR 0x3C9

#ifndef __SP_ASM__

/*
** Start of C-only definitions
*/

/*
** Types
*/

struct BITMAP {
    int width;
    int height;
    int transparent; //color which is substituted as transparency, -1 for no transparency
    int sub_x;
    int sub_y;
    int sub_w; //should be initialized to width
    int sub_h;

    uint8_t *data;
};

typedef struct COLOR {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} COLOR;

struct PALETTE {
    COLOR data[256];
};

/*
** Globals
*/

/*
** Prototypes
*/

void _kgfx_next_context(void);

GFX_CONTEXT * _kgfx_new_context(int pid);
void _kgfx_delete_context(int pid);

void _kgfx_load_palette(PALETTE *palette);

void vsync(void);

void _kgfx_draw_screen(int pid);

void _kgfx_init(void);

#endif

#endif