#include "common.h"

#include "graphics.h"

#include "startup.h"


#include "klib.h"

#include "images.h"

#define INT_ABS(a) (((a) < 0) ? -(a): (a))
#define CLAMP(a, min, max)  ((a) < (min)) ? (min) : (((a) >= (max)) ? (max) : (a))
#define MIN(a, b) ((a) - (b) < 0 ? (b) : (a))

#define NUM_BITMAPS 9
#define NUM_PALETTES 2

#define NUM_SPRITES 2
#define SPRITE_W 20
#define SPRITE_H 20


struct BITMAP {
    int width;
    int height;
    uint8_t transparent; //color which is substituted as transparency
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


uint8_t backbufferdata[VGA_SIZE];
BITMAP bitmaps[NUM_BITMAPS];
PALETTE palette[NUM_PALETTES];


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





//private function


//copies data but skips overwriting 'transparent' values
void transcopy(register uint8_t *dest, register uint8_t *src, register uint32_t len, register uint8_t skipvalue) {
    while (len--) {
        if (*src != skipvalue)
            *dest = *src;
        dest++;
        src++;
    }
}


void loadpalette(PALETTE *palette){
    __outb(VGA_PORT_PALETTE_INDEX, 0); //0 for set all palette colors
    for (int i = 0; i < 256; i++){
        __outb(VGA_PORT_PALETTE_COLOR, palette->data[i].r);
        __outb(VGA_PORT_PALETTE_COLOR, palette->data[i].g);
        __outb(VGA_PORT_PALETTE_COLOR, palette->data[i].b);
    }
}
void randompalette(PALETTE *palette){
    for (int i = 0; i < 256; i++){
        palette->data[i].r = i;
        palette->data[i].g = i;
        palette->data[i].b = i;
    }
}

BITMAP create_bitmap(int width, int height, uint8_t transparent_color, uint8_t *data){
    BITMAP a = {.width = width,
            .height = height,
            .transparent = transparent_color,
            .data = data,
            .sub_x = 0,
            .sub_y = 0,
            .sub_w = width,
            .sub_h = height
    };
    return a;
}

BITMAP create_subbitmap(BITMAP * src, int x, int y, int width, int height){
    if (x < 0 ||
        y < 0 ||
        width < 0 ||
        height < 0 ||
        x + width > src->width ||
        y + height > src->height
            )
    {
        vsync();
        cleartocolor(screen, 10);
        vsync();
        vsync();

        _kpanic("create_subbitmap", "subbitmap larger than parent");
    }


    BITMAP a = *src;
    a.sub_x = x;
    a.sub_y = y;
    a.sub_w = width;
    a.sub_h = height;

    return a;
}



void graphics_init(void){
    screen = &bitmaps[0];
    backbuffer = &bitmaps[1];
    sprite = &bitmaps[2];
    sprite2 = &bitmaps[3];
    screensub1 = &bitmaps[4];
    screensub2 = &bitmaps[5];
    screensub3 = &bitmaps[6];
    screensub4 = &bitmaps[7];
    imatt = &bitmaps[8];

    bitmaps[0] = create_bitmap(SCREEN_W, SCREEN_H, 0, VGA_START);
    bitmaps[1] = create_bitmap(SCREEN_W, SCREEN_H, 0, backbufferdata);
    bitmaps[2] = create_bitmap(SPRITE_W, SPRITE_H, 0, sprites[0]);
    bitmaps[3] = create_bitmap(SPRITE_W, SPRITE_H, '2', sprites[1]);
    bitmaps[4] = create_subbitmap(backbuffer, 5, 5, 150, 90);
    bitmaps[5] = create_subbitmap(backbuffer, 165, 5, 150, 90);
    bitmaps[6] = create_subbitmap(backbuffer, 5, 95, 150, 90);
    bitmaps[7] = create_subbitmap(backbuffer, 165, 95, 150, 90);

    bitmaps[8] = create_bitmap(100, 100, 0, &image_matt);


    //greyscale palette
    for (int i = 0; i < 256; i++){
        palette[0].data[i].r = i;
        palette[0].data[i].g = i;
        palette[0].data[i].b = i;
    }

    //matt palette
    for (int i = 0; i < 256; i++){
        palette[1].data[i].r = palette_matt[i * 3];
        palette[1].data[i].g = palette_matt[i * 3 + 1];
        palette[1].data[i].b = palette_matt[i * 3 + 2];
    }

    greyscale = &palette[0];
    //loadpalette(greyscale);
    //loadpalette(&palette[1]);
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
    if (dest_x + src_x - dest->sub_x < 0) {
        src_x += INT_ABS(dest_x) + dest->sub_x;
        width -= INT_ABS(dest_x) + dest->sub_x;
        dest_x = 0;
    }

    if (dest_x + width > dest->sub_w) {
        width = dest->sub_w - dest_x;
    }

    int dest_size = dest->sub_x + dest->sub_w + (dest->sub_y + dest->sub_h - 1) * dest->width;
    int src_offset = src_x + src->width * src_y;

    for (int row = 0; row < height; row++) {
        int dest_offset = dest_x + dest->sub_x + dest->width * (row + dest_y + dest->sub_y);
        if (dest_offset < 0)
            continue;
        if (dest_offset >= dest_size)
            break;

        transcopy(dest->data + dest_offset, src->data + src_offset + src->width * row, width, src->transparent);
    }
}


void cleartocolor(BITMAP *bmp, uint8_t color) {

    for (int row = bmp->sub_y; row < bmp->sub_h; row++)
        _kmemset(bmp->data + bmp->sub_x + row * bmp->width, bmp->sub_w, color);
}


inline void putpixel(BITMAP *bmp, int x, int y, uint8_t color) {
    if (x < bmp->sub_x || y < bmp->sub_y || x >= bmp->sub_w || y >= bmp->sub_h)
        return;
    //try shifts ie y << 8 + y << 6      320y = 256y + 64y,
    uint8_t *pixel = bmp->data + y * bmp->width + x;
    *pixel = color;
}


void drawline(BITMAP *bmp, int x0, int y0, int x1, int y1, uint8_t color) {
    CLAMP(x0, bmp->sub_x, bmp->sub_w);
    CLAMP(y0, bmp->sub_y, bmp->sub_h);
    CLAMP(x1, bmp->sub_x, bmp->sub_w);
    CLAMP(y1, bmp->sub_y, bmp->sub_h);

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