/*
** SCCS ID:	%W%	%G%
**
** File:	kgraphics.h
**
** Author:	Matthew Cheman mnc3139
**
** Contributor:
**
** Description:	Kernel graphics functions for managing vga driver
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
#define VGA_PORT_VSYNC 0x3DA


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

    uint8_t *data;
};


struct PALETTE {
    uint8_t data[256 * 3];
};


/*
** Globals
*/

BITMAP font;
BITMAP font_white;

/*
** Prototypes
*/

/*
 * _kgfx_next_context - switches the currently displayed graphics context to the next one, if there are
 *     no other contexts, it stays the same
 */
void _kgfx_next_context(void);

/*
 * _kgfx_new_context - creates a new context for a particular process by its pid.
 *
 * if the process already has a context this will return that context and reset it
 */
GFX_CONTEXT *_kgfx_new_context(int pid);

/*
 * _kgfx_detele_context - deletes a context for a particular process by its pid
 */
void _kgfx_delete_context(int pid);

/*
 * _kgfx_load_palette - set the vga driver to use this palette
 */
void _kgfx_load_palette(PALETTE *palette);

/*
 * vsync - waits until a vertical retrace has ended and it is safe to draw a new frame
 */
void vsync(void);

/*
 * _kgfx_draw_screen - if the pid equals the active context being displayed, this draws its backbuffer to the
 *     screen, otherwise it does nothing
 */
void _kgfx_draw_screen(int pid);

/*
 * _kgfx_init - initializes the kgraphics module
 */
void _kgfx_init(void);

#endif

#endif