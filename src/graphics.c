#include "common.h"

#include "graphics.h"

#include "startup.h"


#include "klib.h"

#define INT_ABS(a) (((a) < 0) ? -(a): (a))


uint8_t backbufferdata[VGA_SIZE];

#define NUM_SPRITES 1
#define SPRITE_W 20
#define SPRITE_H 20

uint8_t sprites[NUM_SPRITES][SPRITE_W * SPRITE_H] =
        {{
          "ZZZZZZZZZZZZZZZZZZZZ"
          "ZaaaZZZaaaaZZZaaaaaZ"
          "ZaaaaaaaaaaaZZZaaaaZ"
          "ZaaaaaaaaaaaZZZaaaaZ"
          "ZaaaaaaaaaaZZZaaaaaZ"
          "ZaaaaaaaaaZZZaaaaaaZ"
          "ZaaaaaaaaaaZZZaaaaaZ"
          "ZaaaaaaaaaZZZaaaaaaZ"
          "ZaaaaaaaZZZZaaaaaaaZ"
          "ZaaaaaZZZZZZZZZZZaaZ"
          "ZaZZZZZZZZZZZZZZZZaZ"
          "ZaaaaaaaaaZaaaaaaaaZ"
          "ZaaZaaaaaaaaZaaaaaaZ"
          "ZaaaaaaaaaaaaaZaaaaZ"
          "ZaaaZaaaaaaaaaaZaaaZ"
          "ZaaaaaaaaaaaZaaaaaaZ"
          "ZaaaaaaaaaaaaaaaaaaZ"
          "ZZaaaaaaaaaaaaaaaaaZ"
          "ZaaaaaaaaaaaaaaaaaaZ"
          "ZZZZZZZZZZZZZZZZZZZ"
         }};

struct BITMAP {
    int width;
    int height;
    uint8_t *data;
    //size == width * height
};


BITMAP __screen = {.width = SCREEN_W, .height = SCREEN_H, .data = VGA_START};
BITMAP __backbuffer = {.width = SCREEN_W, .height = SCREEN_H, .data = backbufferdata};


BITMAP *screen = &__screen;
BITMAP *backbuffer = &__backbuffer;

BITMAP __sprite = {.width = SPRITE_W, .height = SPRITE_H, .data = sprites[0]};
BITMAP *sprite = &__sprite;




void vsync() {
    /* wait until any previous retrace has ended */
    do {
    } while (__inb(0x3DA) & 8);

    /* wait until a new retrace has just begun */
    do {
    } while (!(__inb(0x3DA) & 8));

}


void blit(BITMAP *dest, BITMAP *src, int dest_x, int dest_y) {
    if (dest_x + src->width <= 0 ||
        dest_y + src->height <= 0 ||
        dest->width - dest_x <= 0 ||
        dest->height - dest_y <= 0)
        return;

    int destlength = dest->width * dest->height;
    int srcwidthtocopy = src->width;
    int srcstartx = 0;
    //cut off pixels that pass right size of destination
    //in this case simply stop copying pixels once they are outside of dest
    if (src->width >= dest->width - dest_x)
        srcwidthtocopy = dest->width - dest_x;
    //cut off pixels that pass left size of destination
    //in this case we need to start copying from the middle of src
    if (dest_x < 0){
        srcstartx = INT_ABS(dest_x); //location of first pixel overlap inside dest
        srcwidthtocopy = src->width - srcstartx; //copy from srcstartx to end
        dest_x = 0; //must be set to 0 so pixels will not be copied outside of dest
    }

    //copy rows of src into dest
    for (int row = 0; row < src->height; row++) {
        int offset = dest_x + dest->width * (row + dest_y);
        if (offset < 0)
            continue;
        if (offset > destlength)
            break;

        _kmemcpy(dest->data + offset, src->data + srcstartx + row * src->width, srcwidthtocopy);
    }
}

void cleartocolor(BITMAP *dest, uint8_t color) {
//    uint32_t colorblock = ((uint32_t) color & 0xFF) << 24
//                          | ((uint32_t) color & 0xFF) << 16
//                          | ((uint32_t) color & 0xFF) << 8
//                          | (color & 0xFF);
//    for (int p = VGA_START; p < VGA_END; p += sizeof(int)) {
//        *(int *) p = colorblock;
//    }

    _kmemset(dest->data, dest->width * dest->height, color);


}


inline void putpixel(BITMAP *dest, int x, int y, uint8_t color) {


    //try shifts ie y << 8 + y << 6      320y = 256y + 64y,
    uint8_t *pixel = dest->data + y * dest->width + x;

    *pixel = color;
    /*
    if (x > resx)
        x = 300;
    else if (x < 0)
        x = 0;

    if (y > resy)
        y = resy;
    else if (y < 0)
        y = 0;
        */

    /*__asm__("movl	$0x0A0000,%edi\n\t"
        "movb	$0x0F,%al\n\t"
        "movb	%al,(%edi)\n\t");*/
    /*
__asm__("movl	%0,%%edi;"
"movb	%1,%%al;"
"movb	%%al,(%%edi);"
     :
     :"r"(pixel), "r"(color)
     :"%edi"
     );
*/
}


void drawline(BITMAP *dest, int x0, int y0, int x1, int y1, uint8_t color) {

    int swapped = 0;
    if (x0 > x1) {//steep points into left to right order
        int t = x0;
        x0 = x1;
        x1 = t;
        t = y0;
        y0 = y1;
        y1 = t;
        swapped = 1;
    }

    if (y0 == y1) {//check for horizontal lines
        for (; x0 <= x1; x0++)
            putpixel(dest, x0, y0, color);
        return;
    }

    if (x0 == x1) {//check for vertical lines
        if (y1 < y0) {
            int t = y0;
            y0 = y1;
            y1 = t;
        }
        for (; y0 <= y1; y0++)
            putpixel(dest, x0, y0, color);
        return;
    }
    //slope
    int dy = y1 - y0;
    int dx = x1 - x0;

    if (dy == dx) {//check for positive diagonal lines
        while (x0 <= x1) {
            putpixel(dest, x0, y0, color);
            x0++;
            y0++;
        }
        return;
    } else if (dy == -dx) {//check for negative diagonal lines
        while (x0 <= x1) {
            putpixel(dest, x0, y0, color);
            x0++;
            y0--;
        }
        return;
    }

    int de, dne, d;
    int yinc = (dy < 0) ? -1 : 1;

    dy = INT_ABS(dy);
    dx = INT_ABS(dx);

    int steep = (dy > dx) ? 1 : 0;

    if (steep) {
        de = 2 * dx;
        dne = 2 * (dx - dy);
    } else {
        de = 2 * dy;
        dne = 2 * (dy - dx);
    }
    d = 0;
    if (steep && yinc == -1) {
        d = 1;
    }

    while (x0 != x1 || y0 != y1) {
        putpixel(dest, x0, y0, color);

        if (steep)
            y0 += yinc;
        else
            x0++;

        if (d <= 0) {
            d += de;
        } else {
            d += dne;
            if (steep)
                x0++;
            else
                y0 += yinc;
        }
    }
    if (!swapped) //final points from natural left to right lines aren't drawn
        putpixel(dest, x0, y0, color);
}