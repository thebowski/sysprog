#include "common.h"

#include "graphics.h"

#include "startup.h"


#include "klib.h"

#define INT_ABS(a) (((a) < 0) ? -(a): (a))
#define CLAMP(a, min, max)  ((a) < (min)) ? (min) : (((a) >= (max)) ? (max) : (a))


uint8_t backbufferdata[VGA_SIZE];

#define NUM_SPRITES 2
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
                         "ZZZZZZZZZZZZZZZZZZZZ"
         },
         {
                 "|333333333333333333|"
                         "|aaaZZZaaaaZZZaaaaa|"
                         "|333333333333333333|"
                         "|aaaaaaaaaaaZZZaaaa|"
                         "|333333333333333333v"
                         "|aaaaaaaaaZZZaaaaaa|"
                         "|333333333333333333v"
                         "|aaaaaaaaaZZZaaaaaav"
                         "|333333333333333333|"
                         "|aaaaaZZZZZZZZZZZaa|"
                         "|aZZZZZZZZZZZZZZZZa|"
                         "||||||22222222||||||"
                         "||||||2222222222||||"
                         "||||||22222222||||||"
                         "||||||22222222||||||"
                         "||||||22222222||||||"
                         "||||||22222222||||||"
                         "||||||22222222||||||"
                         "||||||||||||||||||||"
                         "||||||||||||||||||||"
         }};

struct BITMAP {
    int width;
    int height;
    uint8_t transparent; //color which is substituted as transparency
    uint8_t *data;

    //size == width * height
};


BITMAP __screen = {.width = SCREEN_W, .height = SCREEN_H, .data = VGA_START};
BITMAP __backbuffer = {.width = SCREEN_W, .height = SCREEN_H, .data = backbufferdata};


BITMAP *screen = &__screen;
BITMAP *backbuffer = &__backbuffer;

BITMAP __sprite = {.width = SPRITE_W, .height = SPRITE_H, .data = sprites[0]};
BITMAP *sprite = &__sprite;

BITMAP __sprite2 = {.width = SPRITE_W, .height = SPRITE_H, .data = sprites[1], .transparent = '2'};
BITMAP *sprite2 = &__sprite2;

//private function
void transcopy(register uint8_t *dest, register uint8_t *src, register uint32_t len, register uint8_t skipvalue) {
    while (len--) {
        if (*src != skipvalue)
            *dest = *src;
        dest++;
        src++;
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

uint8_t get_trans(BITMAP *bmp) {
    return bmp->transparent;
}


void blit(BITMAP *dest, BITMAP *src, int dest_x, int dest_y) {
    blit_ex(dest, src, dest_x, dest_y, 0, 0, src->width, src->height);
}

void blit_ex(BITMAP *dest, BITMAP *src, int dest_x, int dest_y, int src_x, int src_y, int width, int height) {
    if (width <= 0 ||
        height <= 0 ||
        width > src->width ||
        height > src->height ||
        src_x < 0 ||
        src_y < 0 ||
        src_x >= src->width ||
        src_y >= src->height ||
        dest_x + width <= 0 ||
        dest_y + height <= 0 ||
        dest->width - dest_x <= 0 ||
        dest->height - dest_y <= 0)
        return;


    //src is drawn partially outside left of dest, must adjust
    if (dest_x + src_x < 0) {
        src_x += INT_ABS(dest_x);
        width -= INT_ABS(dest_x);
        dest_x = 0;
    }
    else if (dest_x + width > dest->width) {
        width = dest->width - dest_x;
    }

    int dest_size = dest->width * dest->height;
    int src_offset = src_x + src->width * src_y;

    for (int row = 0; row < height; row++) {
        int dest_offset = dest_x + dest->width * (row + dest_y);
        if (dest_offset < 0)
            continue;
        if (dest_offset >= dest_size)
            break;

        transcopy(dest->data + dest_offset, src->data + src_offset + src->width * row, width, src->transparent);
    }
}


void cleartocolor(BITMAP *dest, uint8_t color) {
    _kmemset(dest->data, dest->width * dest->height, color);
}


inline void putpixel(BITMAP *dest, int x, int y, uint8_t color) {
    if (x < 0 || y < 0 || x >= dest->width || y >= dest->height)
        return;
    //try shifts ie y << 8 + y << 6      320y = 256y + 64y,
    uint8_t *pixel = dest->data + y * dest->width + x;
    *pixel = color;
}


void drawline(BITMAP *dest, int x0, int y0, int x1, int y1, uint8_t color) {
    CLAMP(x0, 0, dest->width);
    CLAMP(y0, 0, dest->height);
    CLAMP(x1, 0, dest->width);
    CLAMP(y1, 0, dest->height);

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