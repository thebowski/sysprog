/*
** SCCS ID:	%W%	%G%
**
** File:	graphics.h
**
** Author:	Matthew Cheman mnc3139
**
** Contributor:
**
** Description:	Graphics functions
*/

#include "common.h"

#include "kgraphics.h"
#include "graphics.h"

#include "startup.h"


#include "klib.h"

#define INT_ABS(a) (((a) < 0) ? -(a): (a))
#define CLAMP(a, min, max)  (a) = (((a) < (min)) ? (min) : (((a) >= (max)) ? (max) : (a)))


//copies data but skips overwriting 'transparent' values
void transcopy(register uint8_t *dest, register uint8_t *src, register uint32_t len, register uint8_t skipvalue) {
    while (len--) {
        if (*src != skipvalue)
            *dest = *src;
        dest++;
        src++;
    }
}

void text(BITMAP *dest, char *str, int x, int y, int centered) {
    text_ex(dest, &font, str, x, y, centered);
}

void text_ex(BITMAP *dest, BITMAP *font_image, char *str, int x, int y, int centered) {
    if (centered) {
        char *strcpy = str;
        int offset = 0;
        while (*strcpy) {
            offset += 7;
            strcpy++;
        }
        x -= offset / 2;
    }
    while (*str) {
        blit_ex(dest, font_image, x, y, *str * 8, 0, 8, 8);
        str++;
        x += 7;
    }
}

BITMAP create_bitmap(int width, int height, int transparent_color, uint8_t *data) {
    BITMAP a = {.width = width,
            .height = height,
            .transparent = transparent_color,
            .data = data
    };
    return a;
}

int get_trans(BITMAP *bmp) {
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

    if (dest_x + width > dest->width) {
        width = dest->width - dest_x;
    }

    int dest_size = dest->height * dest->width;
    int src_offset = src_x + src->width * src_y;

    for (int row = 0; row < height; row++) {
        int dest_offset = dest_x + dest->width * (row + dest_y);
        if (dest_offset < 0)
            continue;
        if (dest_offset >= dest_size)
            break;

        uint8_t *d = dest->data + dest_offset;
        uint8_t *s = src->data + src_offset + src->width * row;
        if (src->transparent == -1) //no transparency
            _kmemcpy(d, s, width);
        else
            transcopy(d, s, width, src->transparent);
    }
}


void cleartocolor(BITMAP *bmp, uint8_t color) {

    for (int row = 0; row < bmp->height; row++)
        _kmemset(bmp->data + row * bmp->width, bmp->width, color);
}


inline void putpixel(BITMAP *bmp, int x, int y, uint8_t color) {
    if (x < 0 || y < 0 || x >= bmp->width || y >= bmp->height)
        return;
    //try shifts ie y << 8 + y << 6      320y = 256y + 64y,
    uint8_t *pixel = bmp->data + y * bmp->width + x;
    *pixel = color;
}


void drawline(BITMAP *bmp, int x0, int y0, int x1, int y1, uint8_t color) {
    CLAMP(x0, 0, bmp->width);
    CLAMP(y0, 0, bmp->height);
    CLAMP(x1, 0, bmp->width);
    CLAMP(y1, 0, bmp->height);

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
            putpixel(bmp, x0, y0, color);
        return;
    }

    if (x0 == x1) {//check for vertical lines
        if (y1 < y0) {
            int t = y0;
            y0 = y1;
            y1 = t;
        }
        for (; y0 <= y1; y0++)
            putpixel(bmp, x0, y0, color);
        return;
    }
    //slope
    int dy = y1 - y0;
    int dx = x1 - x0;

    if (dy == dx) {//check for positive diagonal lines
        while (x0 <= x1) {
            putpixel(bmp, x0, y0, color);
            x0++;
            y0++;
        }
        return;
    } else if (dy == -dx) {//check for negative diagonal lines
        while (x0 <= x1) {
            putpixel(bmp, x0, y0, color);
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
        putpixel(bmp, x0, y0, color);

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
        putpixel(bmp, x0, y0, color);
}