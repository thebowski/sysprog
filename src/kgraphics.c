/*
** SCCS ID:	%W%	%G%
**
** File:	?
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	?
*/

#define	__SP_KERNEL__

#include "common.h"
#include "klib.h"
#include "kgraphics.h"
#include "graphics.h"

/*
** PRIVATE DEFINITIONS
*/

#define  MAX_CONTEXTS 25

/*
** PRIVATE DATA TYPES
*/


/*
** PRIVATE GLOBAL VARIABLES
*/

int active_context = 0;
int num_contexts = 0;

int context_pid[MAX_CONTEXTS];
GFX_CONTEXT contexts[MAX_CONTEXTS];
BITMAP backbuffers[MAX_CONTEXTS];
PALETTE palettes[MAX_CONTEXTS];

uint8_t data_backbuffers[MAX_CONTEXTS][VGA_SIZE];

BITMAP screen;

uint8_t default_palette[PALETTE_SIZE] =  {50,50,63,49,49,63,49,49,63,49,48,63,48,48,63,48,47,63,48,47,63,47,46,63,47,46,63,47,46,63,46,45,63,46,45,63,45,44,63,45,44,63,45,43,63,44,43,63,44,42,63,44,42,63,43,42,63,43,41,63,43,41,63,42,40,63,42,40,63,42,39,63,41,39,63,41,38,63,41,38,63,40,37,63,40,37,63,40,36,63,39,36,63,39,35,63,39,35,63,38,34,63,38,34,63,38,33,63,37,33,63,37,33,63,37,32,63,36,32,63,36,31,63,35,31,63,35,30,63,35,30,63,34,29,63,34,29,63,34,28,63,33,28,63,33,27,63,33,27,63,32,26,63,32,26,63,32,25,63,31,25,63,31,25,63,31,24,63,30,24,63,30,23,63,29,23,63,29,22,63,29,22,63,28,21,63,28,21,63,28,21,63,27,20,63,27,20,63,27,19,63,26,19,63,26,18,63,26,18,63,25,17,63,25,17,63,24,16,63,24,16,63,24,15,63,23,15,63,23,14,63,23,14,63,22,13,63,22,13,63,22,12,63,21,12,63,21,12,63,21,11,63,20,11,63,20,10,63,20,10,63,19,9,63,19,9,63,19,8,63,18,8,63,18,7,63,18,7,63,17,6,63,17,6,63,17,5,63,16,5,63,16,4,63,16,4,63,15,4,63,15,3,63,14,3,63,14,2,63,14,2,63,13,1,63,13,1,63,13,0,63,12,0,63,12,0,63,11,0,59,11,0,56,10,0,53,9,0,50,9,0,47,8,0,43,8,0,40,7,0,37,6,0,34,6,0,31,5,0,28,4,0,25,4,0,21,3,0,18,2,0,15,2,0,12,1,0,9,1,0,6,0,0,2,0,0,0,1,0,0,2,1,0,3,2,0,4,2,0,6,3,0,7,4,0,8,5,0,9,5,0,11,6,0,12,7,0,13,8,0,15,8,0,16,9,0,17,10,0,18,11,0,20,11,0,21,12,0,22,13,0,23,14,0,24,15,0,26,15,0,27,16,0,28,17,0,30,18,0,31,18,0,32,19,0,33,20,0,35,21,0,36,21,0,37,22,0,39,23,0,40,23,0,41,24,0,42,25,0,43,26,0,45,27,0,46,27,0,47,28,0,48,29,0,50,30,0,51,30,0,52,31,0,54,32,0,55,33,0,56,33,0,57,34,0,59,35,0,60,36,0,61,36,0,63,37,0,63,38,0,63,38,1,63,38,1,63,39,2,63,39,3,63,39,3,63,40,4,63,40,5,63,40,5,63,41,6,63,41,7,63,41,7,63,42,8,63,42,9,63,42,9,63,42,10,63,43,11,63,43,11,63,43,12,63,44,12,63,44,13,63,44,14,63,45,14,63,45,15,63,45,16,63,46,16,63,46,17,63,46,18,63,47,18,63,47,19,63,47,20,63,48,20,63,48,21,63,48,22,63,49,22,63,49,23,63,49,24,63,50,24,63,50,25,63,50,25,63,51,26,63,51,27,63,51,27,63,52,28,63,52,29,63,52,29,63,53,30,63,53,31,63,53,31,63,54,32,63,54,33,63,54,33,63,55,34,63,55,35,63,55,35,63,56,36,63,56,37,63,56,37,63,57,38,63,57,39,63,57,39,63,58,40,63,58,41,63,58,41,63,59,42,63,59,42,63,59,43,63,60,44,63,60,44,63,60,45,63,61,46,63,61,46,63,61,47,63,62,48,63,62,48};

/*
** PUBLIC GLOBAL VARIABLES
*/


/*
** PRIVATE FUNCTIONS
*/

/*
** PUBLIC FUNCTIONS
*/

void _kgfx_init(void){
    _kmemclr(context_pid, sizeof(context_pid));
    _kgfx_delete_context(0); //delete all contexts
    num_contexts = 0;

    //create screen
    screen = create_bitmap(SCREEN_W, SCREEN_H, 0, VGA_START);

    cleartocolor(&screen, 5*16 + 8);
    //vsync();
}

int offset = 0;
void _kgfx_next_context(void){
    if (num_contexts < 2)
        return;

    //search for next context in use
    for (int i = 0; i < MAX_CONTEXTS; i++){
        active_context++;
        if (active_context >= MAX_CONTEXTS)
            active_context = 0;
        if (context_pid[active_context])
            break;
    }
}

GFX_CONTEXT * _kgfx_new_context(int pid){
    int context = -1;
    //check for existing context for this pid
    for (int i = 0; i < MAX_CONTEXTS; i++) {
        if (context_pid[i] == pid){
            context = i;
            break;
        }
    }

    if (context == -1){
        //take first unused context
        for (int i = 0; i < MAX_CONTEXTS; i++){
            if (!context_pid[i]){
                num_contexts++;
                context_pid[i] = pid;
                context = i;
                break;
            }
        }
        if (context == -1){
            //panic
        }
    }

    _kmemclr(contexts[context].backbuffer->data, VGA_SIZE);
    _kmemcpy(&palettes[context], &default_palette, PALETTE_SIZE);

    active_context = context;

    return &contexts[context];

}

void _kgfx_delete_context(int pid){
    for (int i = 0; i < MAX_CONTEXTS; i++){
        if (context_pid[i] == pid) {
            contexts[i].backbuffer = &backbuffers[i];
            contexts[i].palette = &palettes[i];
            backbuffers[i] = create_bitmap(SCREEN_W, SCREEN_H, 0, data_backbuffers[i]);
            context_pid[i] = 0;
            num_contexts--;
        }
    }
}

void _kgfx_load_palette(PALETTE *palette){
    __outb(VGA_PORT_PALETTE_INDEX, 0); //0 for set all palette colors
    for (int i = 0; i < 256; i++){
        __outb(VGA_PORT_PALETTE_COLOR, palette->data[i].r);
        __outb(VGA_PORT_PALETTE_COLOR, palette->data[i].g);
        __outb(VGA_PORT_PALETTE_COLOR, palette->data[i].b);
    }
}

void vsync() {
    /* wait until any previous retrace has ended */
    do {
    } while (__inb(0x3DA) & 8);

    /* wait until a new retrace has just begun */
    do {
    } while (!(__inb(0x3DA) & 8));

}

void _kgfx_draw_screen(int pid){
    //skip processes that are inactive
    if (pid == context_pid[active_context]){
        vsync();
        _kgfx_load_palette(contexts[active_context].palette);
        blit(&screen, contexts[active_context].backbuffer, 0, 0);

    }
}





















