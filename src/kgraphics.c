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

uint8_t default_palette[PALETTE_SIZE] =
        "1\34\21,\37\35""9($=(\32.*3=49G3-D43u'$77BJ5#F6)h1*f74<CRO@>SB.RB4RB9JDK"
                "bA'`B.\\K6FN^cJH^M?[ML^OF\205DImM9{K?dS<yQ5NYoUYf_Y`qWBkYBzSSzW9wXHq\\=m"
                "[Yp\\Nk[hUczr`GJj\202zfM\207cR\203db\221d;whf~iI\215eH~i[~ia+|\232\224aw"
                "jo}\201mT\232gL\237eads\212\263ab\217nd,\205\244yt|\210sZ[|\222\213tT\207"
                "vU\207r\200\223tQ\305d_\222rr\207vo\212wh\232tc\217ya4\216\255\223{\\\250"
                "uZ\216}\\\257wO\257un\226\200fD\222\2556\226\265N\220\254\225\201r;\226\260"
                "\231\202b\224\201~\255~S\330ns\251~_\255|j\220\202\206\225\202x]\216\247"
                "\230\203n\240~\211\262y\215V\222\250\204\207\223\252\177s\244\200\223\235"
                "\202\224x\214\242\310yv\237\207f\220\207\224\252\201\204\236\207m:\235\274"
                "\233\210x\255\202\200\266\202gC\235\266^\232\261\252\207\220A\243\302J\243"
                "\274\243\217\177p\232\264\250\221v\303\213i\274\214y\272\214\201\306\211"
                "\200\303\214r\274\217n\327\207tI\252\311\246\225\213Q\251\303\361\201|\257"
                "\226s\243\227\224\331\214j\253\227\206\311\225EV\251\323\315\224i\302\224"
                "\222Q\260\320\207\242\275\230\240\253Z\260\312_\256\326w\252\277\257\236"
                "\223\261\240\215\312\231\212\315\233o\324\231z\324\227\216\312\234}\257\242"
                "\235_\265\320X\267\327\251\244\246_\266\333\365\222\206c\271\323\355\230"
                "\221\213\260\312\352\233\204}\264\323n\271\340e\275\335t\272\320g\276\330"
                "\244\260\273z\272\330\347\242\205\342\242\227r\275\330\335\246\205\343\247"
                "v\325\255f\272\257\243l\302\334\314\254\235\334\250\227v\301\335q\302\344"
                "\341\253\177v\302\327\337\262S\300\264\236~\302\343\376\244\235z\305\341"
                "\332\261\217\253\273\311\324\267\200\312\272\225\350\262\214\236\301\324"
                "~\311\345\347\262\224\201\314\350\211\312\347\364\263\246\203\316\352\364"
                "\267\237\216\316\354\355\274\215\343\275\256\355\302a\360\303S\225\320\350"
                "\320\305\253\363\274\241\243\316\342\254\314\334\360\277\227\361\275\251"
                "\270\312\324\354\301\252\357\302\242\232\325\356\346\310\261\320\314\314"
                "\356\312\250\241\333\363\352\317\231\370\320h\375\310\264\247\334\357\346"
                "\321\250\334\323\270\372\315\257\350\323\311\372\320\270\372\331~\373\324"
                "\305\367\331\306\373\336\216\364\332\320\372\333\300\363\340\270\373\342"
                "\231\353\343\313\327\350\357\375\343\316\365\350\307\374\350\266\376\351"
                "\256\365\356\330\377\357\306\366\361\342\367\364\356\377\377\377";

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
        blit(&screen, contexts[active_context].backbuffer, 0, 0);

    }
}





















