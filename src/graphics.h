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

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


typedef struct PALETTE PALETTE;
typedef struct BITMAP BITMAP;

typedef struct GFX_CONTEXT {
    BITMAP *backbuffer;
    PALETTE *palette;
} GFX_CONTEXT;


/*
 * create_bitmap - creates a BITMAP structure from passed parameters
 *
 * data is a pointer to the location of the bitmap data which should be of size width * height
 * transparent_color is either -1 for no transparency or 0 - 255 for the color to be treated as transparent
 *     when drawing bitmap onto other bitmaps
 *
 */
BITMAP create_bitmap(int width, int height, int transparent_color, uint8_t *data);

/*
 * putpixel - put a pixel with the specified color at the coordinates (x,y) in the dest bitmap
 *
 * if the coordinates are outside dest, this function does nothing
 */
void putpixel(BITMAP *dest, int x, int y, uint8_t color);

/*
 * cleartocolor - clears the entire bitmap dest to one color
 */
void cleartocolor(BITMAP *dest, uint8_t color);

/*
 * drawline - draws a straight line on dest with the specified color from (x0,y0) to (x1,y1)
 *
 * if the line extends beyond dest, that part is not drawn
 */
void drawline(BITMAP *dest, int x0, int y0, int x1, int y1, uint8_t color);

/*
 * blit - draws a bitmap src onto dest with it positioned with src's upper left corner at (dest_x, dest_y)
 *
 * parts of the src bitmap which lie outside of dest will not be drawn
 * if the src bitmap has a transparency color, those pixels will not be drawn to dest
 */
void blit(BITMAP *dest, BITMAP *src, int dest_x, int dest_y);

/*
 * blit_ex - draws a section of bitmap src onto dest
 *
 * a region of src starting with the upper left corner as (src_x, src_y) and extending by width and height
 *     is drawn onto dest at (dest_x, dest_y)
 * if the src bitmap has a transparency color, those pixels will not be drawn to dest
 */
void blit_ex(BITMAP *dest, BITMAP *src, int dest_x, int dest_y, int src_x, int src_y, int width, int height);

/*
 * get_trans - returns the color in bmp which is treated as transparent or -1 if the bitmap has no transparency
 */
int get_trans(BITMAP *bmp);

/*
 * text - draws str text on dest
 *
 * text will be drawn with it's upper left corner at (x,y) if centered is 0
 * text will be drawn with the center of the string at x and it's top at y if centered is 1
 * each character is 7 pixels wide by 8 pixels tall
 *
 */
void text(BITMAP *dest, char *str, int x, int y, int centered);

/*
 * text_ex - draws str text on dest with a definable font
 *
 * text will be drawn with it's upper left corner at (x,y) if centered is 0
 * text will be drawn with the center of the string at x and it's top at y if centered is 1
 * each character is 7 pixels wide by 8 pixels tall
 *
 * a font must be passed in and the image must be 1024 * 8 resolution with each character at it's ascii code * 8
 *
 */
void text_ex(BITMAP *dest, BITMAP *font_image, char *str, int x, int y, int centered);


#endif