/*
** SCCS ID:	@(#)user.c	1.1	4/22/16
**
** File:	user.c
**
** Author:	CSCI-452 class of 20155
**
** Contributor:
**
** Description:	User routines.
*/

#include "common.h"
#include "diskdriver.h"
#include "user.h"

#include "graphics.h"

#include "images.h"

/*
** USER PROCESSES
**
** Each is designed to test some facility of the OS:
**
**	User(s)		Tests/Features
**	=======		===============================================
**	A, B, C		Basic operation
**	D		Spawns Z and exits
**	E, F, G		Sleep for different lengths of time
**	H		Doesn't call exit()
**	J		Tries to spawn 2*N_PROCS copies of Y
**	K		Spawns several copies of X, sleeping between
**	L		Spawns several copies of X, preempting between
**	M		Spawns W three times, reporting PIDs
**	N		Like M, but spawns X and W
**	P		Iterates three times, sleeping & printing system time
**	Q		Tries to execute a bogus system call (bad code)
**	R		Reading and writing three characters
**	S		Loops forever, sleeping 30 seconds at a time
**	T		Loops, spawning W and X procs then waiting
**	U		Spawns W three times, sleeps, then kills them
**
**	W, X, Y, Z	Print characters (spawned by other processes)
**
** Output from user processes is always alphabetic.  Uppercase 
** characters are "expected" output; lowercase are "erroneous"
** output.
**
** More specific information about each user process can be found in
** the header comment for that function (below).
**
** To spawn a specific user process, uncomment its SPAWN_x
** definition in the user.h header file.
*/

/*
** Prototypes for all one-letter user main routines (even
** ones that may not exist, for completeness)
*/

int32_t user_a(void);

int32_t user_b(void);

int32_t user_c(void);

int32_t user_d(void);

int32_t user_e(void);

int32_t user_f(void);

int32_t user_g(void);

int32_t user_h(void);

int32_t user_i(void);

int32_t user_j(void);

int32_t user_k(void);

int32_t user_l(void);

int32_t user_m(void);

int32_t user_n(void);

int32_t user_o(void);

int32_t user_p(void);

int32_t user_q(void);

int32_t user_r(void);

int32_t user_s(void);

int32_t user_t(void);

int32_t user_u(void);

int32_t user_v(void);

int32_t user_w(void);

int32_t user_x(void);

int32_t user_y(void);

int32_t user_z(void);

/*
** Users A, B, and C are identical, except for the character they
** print out via swrites().  Each prints its ID, then loops 30
** times delaying and printing, before exiting.  They also verify
** the status return from swrites().
*/

#define SPRITE_W 20
#define SPRITE_H 20

uint8_t sprites[2][SPRITE_W * SPRITE_H] =
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


int32_t user_d(void) {
    BITMAP sprite = create_bitmap(SPRITE_W, SPRITE_H, 0, sprites[0]);

    BITMAP sprite2 = create_bitmap(SPRITE_W, SPRITE_H, '2', sprites[1]);

    BITMAP imatt = create_bitmap(100, 100, -1, image_matt);

    GFX_CONTEXT *ctx = getgfxcontext();
    ctx->palette = palette_matt;

    int x = 0, y = 0, dir = 0;
    int sx = 0, sy = 0, sy2 = 0;
    float sx2 = 0;
    int xdir = 1, ydir = 1;

    char color2 = 0;
    char bgcolor = 200;
    int delay = 0;
    while (1) {


        cleartocolor(ctx->backbuffer, bgcolor);


        if (delay > 50) {
            bgcolor++;

            delay = 0;
            if (bgcolor == get_trans(ctx->backbuffer))
                delay = -1000;
        }
        delay++;

        char color = 0;
        for (int j = 0; j < 240; j += 16) {
            for (int i = 0; i < 320; i += 16) {
                putpixel(ctx->backbuffer, i, j, color++);
            }

        }

        for (int j = 0; j < 16; j++) {
            for (int i = 0; i < 16; i++) {
                putpixel(ctx->backbuffer, 300 + i, 41 + j, i + j * 16);
            }

        }

        text(ctx->backbuffer, "HEY THIS IS WORKING.", 200, 10, 1);

        sx += xdir;
        sy += ydir;

        if (sx > 350) {
            sx = 350;
            xdir = -1;
        }
        else if (sx < -50) {
            sx = -50;
            xdir = 1;
        }
        if (sy > 250) {
            sy = 250;
            ydir = -1;
        } else if (sy < -50) {
            sy = -50;
            ydir = 1;
        }

        sx2 += .5;
        if (sx2 > 320) {
            sx2 = -20;
            sy2 += 16;
        }
        if (sy2 > 200) {
            sy2 = 0;
        }

        blit(ctx->backbuffer, &imatt, 60, 60);

        drawline(ctx->backbuffer, 160, 90, x, y, color2++);
        drawline(ctx->backbuffer, sx, sy, (int) sx2, sy2, color2);
        drawline(ctx->backbuffer, sx2, sy2, 320 - (int) sx2, 200 - sy2, 255 - color2);

        blit(ctx->backbuffer, &sprite, sx, sy);

        blit(ctx->backbuffer, &sprite2, (int) sx2, sy2);

        blit(ctx->backbuffer, &sprite2, 320 - (int) sx2, 200 - sy2);

        //180?




        if (dir == 0) x++;
        if (dir == 1) y++;
        if (dir == 2) x--;
        if (dir == 3) y--;

        if (x > 319) {
            dir = 1;
            x--;
        }
        if (y > 200) {
            dir = 2;
            y--;
        }
        if (x < 0) {
            dir = 3;
            x++;
        }
        if (y < 0) {
            dir = 0;
            y++;
        }


        blit_ex(ctx->backbuffer, ctx->backbuffer, 280, 0, 140, 80, 40, 40);
        drawline(ctx->backbuffer, 280, 0, 280, 40, 0);
        drawline(ctx->backbuffer, 280, 40, 320, 40, 0);


        drawscreen();
    }
}

int32_t user_me( void) {
    return disk_demo();
}

int32_t user_a( void ) {
	int i, j;
	int32_t status;
	char buf[12];

	status = swrites( "A", 1 );
	if( status != SUCCESS ) {
		cwrites( "User A, write 1 status ", 23 );
		i = cvt_dec( buf, status );
		cwrites( buf, i );
		cwrites( "\n", 1 );
	}
	for( i = 0; i < 10; ++i ) {
		for( j = 0; j < DELAY_STD; ++j )
			continue;
		status = swrites( "A", 1 );
		if( status != SUCCESS ) {
			cwrites( "User A, write 2 status ", 23 );
			i = cvt_dec( buf, status );
			cwrites( buf, i );
			cwrites( "\n", 1 );
		}
	}

	exit( EXIT_SUCCESS );

	status = swrites( "a", 1 );	/* shouldn't happen! */
	if( status != SUCCESS ) {
		cwrites( "User A, write 3 status ", 23 );
		i = cvt_dec( buf, status );
		cwrites( buf, i );
		cwrites( "\n", 1 );
	}

	return( 0 );  // shut the compiler up!
}

int32_t user_b(void) {
    GFX_CONTEXT *ctx = getgfxcontext();


    int color = 0;
    while (1) {

        cleartocolor(ctx->backbuffer, color++);
        if (color > 253)
            color = 0;

        drawscreen();
    }
}

int32_t user_c(void) {
    GFX_CONTEXT *ctx = getgfxcontext();


    while (1) {

        cleartocolor(ctx->backbuffer, 16);


        char color = 0;
        for (int j = 0; j < 240; j += 16) {
            for (int i = 0; i < 320; i += 16) {
                putpixel(ctx->backbuffer, i, j, color++);

            }
            for (int a = 0; a < 200; a += 2) {
                putpixel(ctx->backbuffer, 100, a, color);
            }
            for (int a = 0; a < 200; a += 4) {
                putpixel(ctx->backbuffer, 101, a, color);
            }
            for (int a = 0; a < 200; a += 8) {
                putpixel(ctx->backbuffer, 102, a, color);
            }
        }

        for (int j = 0; j < 16; j++) {
            for (int i = 0; i < 16; i++) {
                putpixel(ctx->backbuffer, 40 + i, 40 + j, i + j * 16);
            }

        }

        text(ctx->backbuffer, "Hello World!", 100, 100, 0);

        drawscreen();
    }
}

/*
** User D spawns user Z, then exits before it can terminate.
*/

int32_t user_f(void) {


    GFX_CONTEXT *ctx = getgfxcontext();

    BITMAP jupiter = create_bitmap(320, 200, -1, jupiter_image);
    ctx->palette = jupiter_palette;

    blit(ctx->backbuffer, &jupiter, 0, 0);
    while (1) {


        drawscreen();
    }


}


#define PRES_TITLE(str) text_ex(ctx->backbuffer, &font_white, str, header_x, header_y, 1);
#define PRES_LINE(str) text(ctx->backbuffer, str, body_x, body_y + offset, 0); offset += body_step;


void draw_card(BITMAP *dest, int dest_x, int dest_y, int w, int h, uint8_t color, uint8_t shadow_color,
               uint8_t shadow_color_light) {

    //card
    for (int y = dest_y; y < dest_y + h; y++) {
        for (int x = dest_x; x < dest_x + w; x++) {
            putpixel(dest, x, y, color);
        }
    }

    //shadows
    for (int x = dest_x + 2; x < dest_x + w + 1; x++) {
        putpixel(dest, x, dest_y + h, shadow_color);
        putpixel(dest, x, dest_y + h + 1, shadow_color_light);
    }
    for (int y = dest_y + 2; y < dest_y + h + 1; y++) {
        putpixel(dest, dest_x + w, y, shadow_color);
        putpixel(dest, dest_x + w + 1, y, shadow_color_light);
    }
    putpixel(dest, dest_x + w + 1, dest_y + h + 1, shadow_color_light);
    putpixel(dest, dest_x + w, dest_y + 2, shadow_color_light);
    putpixel(dest, dest_x + 2, dest_y + h, shadow_color_light);
}

/*
** Users E, F, and G test the sleep facility.
**
** User E sleeps for 10 seconds at a time.
*/

uint8_t background_data[SCREEN_W * SCREEN_H];
uint8_t namecard_data[SCREEN_W * SCREEN_H];

int32_t user_e(void) {

    int spacing = 8;

    int titlecard_w = (320 - spacing * 2);
    int titlecard_h = (spacing * 3);
    int titlecard_x = spacing;
    int titlecard_y = spacing;


    int bodycard_w = titlecard_w;
    int bodycard_h = (200 - (spacing * 3) - titlecard_h);
    int bodycard_x = titlecard_x;
    int bodycard_y = (titlecard_h + titlecard_y + spacing);


    GFX_CONTEXT *ctx = getgfxcontext();

    BITMAP background = create_bitmap(SCREEN_W, SCREEN_H, -1, background_data);
    BITMAP namecard = create_bitmap(SCREEN_W, SCREEN_H, -1, namecard_data);

    // text color
    ctx->palette->data[0] = 10;
    ctx->palette->data[1] = 10;
    ctx->palette->data[2] = 10;

    //body
    ctx->palette->data[3] = 63;
    ctx->palette->data[4] = 63;
    ctx->palette->data[5] = 63;

    //title
    ctx->palette->data[6] = 19;
    ctx->palette->data[7] = 26;
    ctx->palette->data[8] = 60;

    //background
    ctx->palette->data[9] = 51;
    ctx->palette->data[10] = 52;
    ctx->palette->data[11] = 60;

    //shadow
    ctx->palette->data[12] = ctx->palette->data[9] / 2;
    ctx->palette->data[13] = ctx->palette->data[10] / 2;
    ctx->palette->data[14] = ctx->palette->data[11] / 2;

    //shadow light
    ctx->palette->data[15] = ctx->palette->data[9] / 4 * 3;
    ctx->palette->data[16] = ctx->palette->data[10] / 4 * 3;
    ctx->palette->data[17] = ctx->palette->data[11] / 4 * 3;


    uint8_t body_color = 1;
    uint8_t title_color = 2;
    uint8_t back_color = 3;
    uint8_t shadow_color = 4;
    uint8_t shadow_color_light = 5;

    int header_x = 320 / 2;
    int header_y = titlecard_y + spacing;

    int body_x = bodycard_x + spacing;
    int body_y = bodycard_y + spacing;

    int body_step = 16;


    cleartocolor(&background, back_color);
    cleartocolor(&namecard, back_color);

    draw_card(&background, titlecard_x, titlecard_y, titlecard_w, titlecard_h, title_color, shadow_color,
              shadow_color_light);
    draw_card(&background, bodycard_x, bodycard_y, bodycard_w, bodycard_h, body_color, shadow_color,
              shadow_color_light);


    int projtitle_h = 120;
    draw_card(&namecard, spacing, spacing, titlecard_w, projtitle_h, title_color, shadow_color, shadow_color_light);

    int namecardsize = (titlecard_w - 2 * spacing) / 3;

    draw_card(&namecard,
              spacing,
              projtitle_h + spacing * 2,
              namecardsize,
              7 * spacing,
              title_color, shadow_color, shadow_color_light);

    draw_card(&namecard,
              spacing * 2 + namecardsize,
              projtitle_h + spacing * 2,
              namecardsize,
              7 * spacing,
              title_color, shadow_color, shadow_color_light);

    draw_card(&namecard,
              spacing * 3 + namecardsize * 2,
              projtitle_h + spacing * 2,
              namecardsize,
              7 * spacing,
              title_color, shadow_color, shadow_color_light);


    char buf[] = " ";


    int MAX_SLIDE = 11;
    int MIN_SLIDE = -6;
    int slide = MIN_SLIDE;

    while (1) {

        int offset = 0;
        blit(ctx->backbuffer, &background, 0, 0);

        switch (slide) {
            case -6:
                blit(ctx->backbuffer, &namecard, 0, 0);
                text_ex(ctx->backbuffer, &font_white, "Operating System OS", header_x, spacing + projtitle_h / 2, 1);

                text_ex(ctx->backbuffer, &font_white, "Matthew", spacing + namecardsize / 2, projtitle_h + spacing * 4,
                        1);
                text_ex(ctx->backbuffer, &font_white, "Cheman", spacing + namecardsize / 2, projtitle_h + spacing * 6,
                        1);

                text_ex(ctx->backbuffer, &font_white, "Neil", spacing + namecardsize / 2 + (spacing + namecardsize),
                        projtitle_h + spacing * 4, 1);
                text_ex(ctx->backbuffer, &font_white, "Guertin", spacing + namecardsize / 2 + (spacing + namecardsize),
                        projtitle_h + spacing * 6, 1);

                text_ex(ctx->backbuffer, &font_white, "Alexander",
                        spacing + namecardsize / 2 + (spacing + namecardsize) * 2, projtitle_h + spacing * 4, 1);
                text_ex(ctx->backbuffer, &font_white, "Bobowski",
                        spacing + namecardsize / 2 + (spacing + namecardsize) * 2, projtitle_h + spacing * 6, 1);
                break;

            case -5:
                PRES_TITLE("Project Goal")
                PRES_LINE("Display Images")
                PRES_LINE("")
                PRES_LINE("Save/Load images to disk")
                PRES_LINE("")
                PRES_LINE("Send/Receive images over network")
                PRES_LINE("")
                PRES_LINE("Enter commands through terminal")
                break;

            case -4:
                PRES_TITLE("Network - 3 Layers")
                PRES_LINE("User Space")
                PRES_LINE("  Request port, relinquish port, send")
                PRES_LINE("    packet, receive packet")
                PRES_LINE("Operating System")
                PRES_LINE("  Send packet, assign ports, distribute")
                PRES_LINE("    received packets")
                PRES_LINE("Driver")
                PRES_LINE("  Send packet, receive packet")
                break;

            case -3:
                PRES_TITLE("Network - User Space")
                PRES_LINE("Request and relinquish ports from OS")
                PRES_LINE(" -Ask for port with port number, pass")
                PRES_LINE("    receive function pointer")
                PRES_LINE("Send Packet")
                PRES_LINE(" -Include port to send over,")
                PRES_LINE("    destination IP, packet pointer")
                PRES_LINE("Libraries")
                PRES_LINE(" -Libraries for creating packets, ICMP")
                PRES_LINE("    definitely, UDP and TCP planned")
                break;

            case -2:
                PRES_TITLE("Network - OS Space")
                PRES_LINE("OS handles endian conversion,")
                PRES_LINE("  IP header creation and port verification")
                PRES_LINE("")
                PRES_LINE("Contains list of ports and assigns them to")
                PRES_LINE("  processes, passes received packets to")
                PRES_LINE("  respective ports")
                PRES_LINE("")
                PRES_LINE("Tells network driver to send packets")
                break;

            case -1:
                PRES_TITLE("Network - Driver Space")
                PRES_LINE("Handles setup of the network card")
                PRES_LINE("")
                PRES_LINE("Sends packets provided by OS over network")
                PRES_LINE("")
                PRES_LINE("Read received packets into circular")
                PRES_LINE("  then OS distributes them to their")
                PRES_LINE("  associated processes")
                //PRES_LINE("345678901234567890123456789012345678901")
                break;

            case 0:
                PRES_TITLE("Disk Driver")
                PRES_LINE("Control Communication")
                PRES_LINE("  Hard coded legacy addresses")
                PRES_LINE("  PCI bus")
                PRES_LINE("")
                PRES_LINE("Data Communication")
                PRES_LINE("  PIO")
                PRES_LINE("  DMA")
                PRES_LINE("")
                PRES_LINE("Use FAT12 filesystem")
                break;

            case 1:
                PRES_TITLE("Data Communication")
                PRES_LINE("PIO")
                PRES_LINE("  Polled")
                PRES_LINE("  Slow")
                PRES_LINE("  16 bits at a time")
                PRES_LINE("")
                PRES_LINE("DMA")
                PRES_LINE("  Physical Region Descriptor Table")
                PRES_LINE("  Can send directly to memory")
                PRES_LINE("    used by graphics")
                break;

            case 2:
                PRES_TITLE("Serial IO")
                PRES_LINE("Use terminal")
                PRES_LINE("  Enter commands")
                PRES_LINE("  Display messages")
                PRES_LINE("")
                PRES_LINE("Shell")
                PRES_LINE("  Parse")
                PRES_LINE("  Execute")
                break;

            case 3:
                PRES_TITLE("Graphics")
                PRES_LINE("Mode 13h VGA")
                PRES_LINE("")
                PRES_LINE("Accomplished by real mode bios interrupt")
                break;

            case 4:
                PRES_TITLE("Driver Capabilities")
                PRES_LINE("Resolution of 320x200")
                PRES_LINE("")
                PRES_LINE("8 bit indexed color")
                PRES_LINE("")
                PRES_LINE("Palette switching -> 18 bits to pick from")
                PRES_LINE("")
                PRES_LINE("Vsync")
                break;

            case 5:
                PRES_TITLE("Graphics System Structure")
                PRES_LINE("2 syscall interface")
                PRES_LINE("  Request a graphics context")
                PRES_LINE("  Draw context to screen")
                PRES_LINE("")
                PRES_LINE("Kernel tab intercept switches between")
                PRES_LINE("  running graphics contexts")
                PRES_LINE("")
                PRES_LINE("User facing library of graphics functions")
                break;

            case 6:
                PRES_TITLE("Graphics Library")
                PRES_LINE("Provides functions for easy manipulation")
                PRES_LINE("  of graphics")
                PRES_LINE("")
                PRES_LINE("putpixel")
                PRES_LINE("cleartocolor")
                PRES_LINE("drawline")
                PRES_LINE("blit")
                PRES_LINE("text")
                break;

            case 7:
                PRES_TITLE("Blit")
                PRES_LINE("Can draw any rectangular region of source")
                PRES_LINE("  bitmap to any region of destination")
                PRES_LINE("")
                PRES_LINE("Handles clipping")
                PRES_LINE("")
                PRES_LINE("Transparency")
                break;

            case 8:
                PRES_TITLE("Text")
                PRES_LINE("text(BITMAP * dst, BITMAP * font,")
                PRES_LINE("  char * str, int x, int y) {")
                PRES_LINE("  while (*str) {")
                PRES_LINE("    blit(dst,font,x,y,*str * 8,0,8,8);")
                PRES_LINE("    str++;")
                PRES_LINE("    x += 8;")
                PRES_LINE("  }")
                PRES_LINE("}")
                break;

            case 9:
                PRES_TITLE("Fun")
                PRES_LINE("Turns out VGA palettes are 6 bits per")
                PRES_LINE("  pixel not 8")
                PRES_LINE("")
                PRES_LINE("As the OS grows in size, will statically")
                PRES_LINE("  allocated structures eventually overlap")
                PRES_LINE("  VGA memory?")
                break;

            case 10:
                PRES_TITLE("Fun")
                PRES_LINE("Turns out VGA palettes are 6 bits per")
                PRES_LINE("  pixel not 8")
                PRES_LINE("")
                PRES_LINE("As the OS grows in size, will statically")
                PRES_LINE("  allocated structures eventually overlap")
                PRES_LINE("  VGA memory?")
                PRES_LINE("")
                PRES_LINE("Yes.")
                break;

            case 11:
                PRES_TITLE("THE END.")
                PRES_LINE("Questions?")
                break;

        }

        text(ctx->backbuffer, buf, 0, 0, 0);
        drawscreen();
        int chr = c_getchar();


        if (chr == '6') {
            slide++;
        } else if (chr == '4') {
            slide--;
        } else {
            buf[0] = (char) chr;
        }

        if (slide < MIN_SLIDE) {
            slide = MIN_SLIDE;
        }
        if (slide > MAX_SLIDE) {
            slide = MAX_SLIDE;
        }


    }
}


/*
** User G sleeps for 15 seconds at a time.
*/

int32_t user_g(void) {
    int i;
    int32_t pid;
    char buf[12];

    pid = getpid();
    cwrites("User G (", 8);
    i = cvt_dec(buf, pid);
    cwrites(buf, i);
    cwrites(") running\n", 10);
    swrites("G", 1);
    for (i = 0; i < 5; ++i) {
        sleep(15);
        swrites("G", 1);
    }

    exit(EXIT_SUCCESS);
    return (0);  // shut the compiler up!

}


/*
** User H is like A-C except it only loops 5 times and doesn't
** call exit().
*/

int32_t user_h(void) {
    int i, j;

    swrites("H", 1);
    for (i = 0; i < 5; ++i) {
        for (j = 0; j < DELAY_STD; ++j)
            continue;
        swrites("H", 1);
    }

    cwrite("User H returning without exiting!\n");

    return (0);  // shut the compiler up!

}


/*
** User J tries to spawn 2*N_PROCS copies of user_y.
*/

int32_t user_j(void) {
    int i;
    int pid;

    swrites("J", 1);

    for (i = 0; i < N_PROCS * 2; ++i) {
        pid = fork();
        if (pid < 0) {
            cwrite("User J fork() failed\n");
        } else if (pid == 0) {
            exec(user_y);
            cwrite("User J, exec(Y) failed\n");
            exit(EXIT_FAILURE);
        } else {
            swrites("J", 1);
        }
    }

    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

}

/*
** User K prints, goes into a loop which runs three times, and exits.
** In the loop, it does a spawn of user_x, sleeps 30 seconds, and prints.
*/

int32_t user_k(void) {
    int i;
    int pid;

    swrites("K", 1);

    for (i = 0; i < 3; ++i) {
        pid = fork();
        if (pid < 0) {
            cwrite("User K fork() failed\n");
        } else if (pid == 0) {
            exec(user_x);
            cwrite("User K, exec(X) failed\n");
            exit(EXIT_FAILURE);
        } else {
            sleep(30);
            swrites("K", 1);
        }
    }

    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

}


/*
** User L is like user K, except that it prints times and doesn't sleep
** each time, it just preempts itself.
*/

int32_t user_l(void) {
    int i;
    int pid;
    uint32_t time;
    char buf[8];

    time = gettime();
    cwrite("User L running, initial time ");
    i = cvt_hex(buf, time);
    cwrites(buf, i);
    cwrites("\n", 1);

    swrites("L", 1);

    for (i = 0; i < 3; ++i) {
        pid = fork();
        if (pid < 0) {
            cwrite("User L fork() failed\n");
        } else if (pid == 0) {
            exec(user_x);
            cwrite("User L, exec(X) failed\n");
            exit(EXIT_FAILURE);
        } else {
            // yield, but don't sleep
            sleep(0);
            swrites("L", 1);
        }
    }

    time = gettime();
    cwrite("User L exiting, time ");
    i = cvt_hex(buf, time);
    cwrites(buf, i);
    cwrites("\n", 1);

    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

}


/*
** User M iterates spawns three copies of user W,
** reporting their PIDs.
*/

int32_t user_m(void) {
    int i;
    int pid;
    char buf[12];

    swrites("M", 1);

    for (i = 0; i < 3; ++i) {
        pid = fork();
        if (pid < 0) {
            cwrite("User M fork() failed\n");
        } else if (pid == 0) {
            exec(user_w);
            cwrite("User M, exec(W) failed\n");
            exit(EXIT_FAILURE);
        } else {
            cwrite("User M spawned W, PID ");
            i = cvt_dec(buf, pid);
            cwrites(buf, i);
            cwrites("\n", 1);
            swrites("M", 1);
        }
    }

    exit(EXIT_SUCCESS);
    return (0);  // shut the compiler up!

}


/*
** User N is like user M, except that it spawns user W and user Z
*/

int32_t user_n(void) {
    int i;
    int pid;
    char buf[12];

    swrites("N", 1);

    for (i = 0; i < 3; ++i) {
        pid = fork();
        pid = fork();
        if (pid < 0) {
            cwrite("User N fork() failed\n");
        } else if (pid == 0) {
            exec(user_w);
            cwrite("User N, exec(W) failed\n");
            exit(EXIT_FAILURE);
        } else {
            cwrite("User N spawned W, PID ");
            i = cvt_dec(buf, pid);
            cwrites(buf, i);
            cwrites("\n", 1);
        }

        pid = fork();
        pid = fork();
        if (pid < 0) {
            cwrite("User N fork() failed\n");
        } else if (pid == 0) {
            exec(user_z);
            cwrite("User N, exec(Z) failed\n");
            exit(EXIT_FAILURE);
        } else {
            cwrite("User N spawned Z, PID ");
            i = cvt_dec(buf, pid);
            cwrites(buf, i);
            cwrites("\n", 1);
        }
        swrites("N", 1);
    }

    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

}


/*
** User P iterates three times.  Each iteration sleeps for two seconds,
** then gets and prints the system time.
*/

int32_t user_p(void) {
    uint32_t time;
    int i;
    char buf[8];

    cwrite("User P running, start at ");
    time = gettime();
    i = cvt_hex(buf, time);
    cwrites(buf, i);
    cwrites("\n", 1);

    swrites("P", 1);

    for (i = 0; i < 3; ++i) {
        sleep(2);
        time = gettime();
        cwrite("User P reporting time ");
        i = cvt_hex(buf, time);
        cwrites(buf, i);
        cwrites("\n", 1);
        swrites("P", 1);
    }

    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

}


/*
** User Q does a bogus system call
*/

int32_t user_q(void) {

    swrites("Q", 1);
    bogus();
    cwrite("User Q returned from bogus syscall!?!?!\n");
    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

}


/*
** User R loops 3 times reading/writing, then exits.
*/

int32_t user_r(void) {
    int i, j;
    char ch = '&';
    char buf[12];

    sleep(10);
    for (i = 0; i < 3; ++i) {
        do {
            swrites("R", 1);
            ch = sreadch();
            if (ch < 0) {
                cwrite("User R, read returned ");
                j = cvt_dec(buf, ch);
                cwrites(buf, j);
                cwrites("\n", 1);
            } else if (ch == -1) {    /* wait a bit */
                sleep(1);
            }
        } while (ch < 0);
        swrites(&ch, 1);
    }

    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

}


/*
** User S sleeps for 20 seconds at a time, and loops forever.
*/

int32_t user_s(void) {

    swrites("S", 1);
    for (; ;) {
        sleep(20);
        swrites("S", 1);
    }

    cwrite("User S exiting!?!?!n");
    swrites("s", 1);
    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

}

/*
** User T iterates three times, spawning copies of user W; then it
** sleeps for eight seconds, and then waits for those processes.
*/

int32_t user_t(void) {
    int i, j;
    int pid[3];
    int32_t status;
    char buf[12];


    for (i = 0; i < 3; ++i) {
        pid[i] = fork();
        if (pid[i] < 0) {
            swrites("t", 1);
        } else if (pid[i] == 0) {
            exec(user_w);
            cwrite("User T (");
            j = cvt_dec(buf, getpid());
            cwrites(buf, j);
            cwrite(" exec(W) failed\n");
            exit(EXIT_FAILURE);
        } else {
            cwrite("User T spawned W, PID ");
            j = cvt_dec(buf, pid[i]);
            cwrites(buf, j);
            cwrites("\n", 1);
            swrites("T", 1);
        }
    }

    sleep(8);

    // collect exit status information

    do {
        status = wait();
        if (status == E_NO_PROCS) {
            break;
        }
        cwrite("User T: child exit status ");
        j = cvt_dec(buf, status);
        cwrites(buf, j);
        cwrites("\n", 1);
    } while (1);

    exit(EXIT_SUCCESS);
    return (0);  // shut the compiler up!

}


/*
** User U is like user T, except that it kills any living children
** after sleeping.
*/

int32_t user_u(void) {
    int i, j;
    int pid[3];
    int32_t status;
    char buf[12];


    for (i = 0; i < 3; ++i) {
        pid[i] = fork();
        if (pid[i] < 0) {
            swrites("u", 1);
        } else if (pid[i] == 0) {
            exec(user_w);
            cwrite("User U (");
            j = cvt_dec(buf, getpid());
            cwrites(buf, j);
            cwrite(" exec(W) failed\n");
            exit(EXIT_FAILURE);
        } else {
            cwrite("User U spawned W, PID ");
            j = cvt_dec(buf, pid[i]);
            cwrites(buf, j);
            cwrites("\n", 1);
            swrites("U", 1);
        }
    }

    sleep(8);

    for (i = 0; i < 3; ++i) {
        if (pid[i] > 0) {
            status = kill(pid[i]);
            cwrite("User U kill PID ");
            j = cvt_dec(buf, pid[i]);
            cwrites(buf, j);
            cwrite(" status ");
            j = cvt_dec(buf, status);
            cwrites(buf, j);
            cwrites("\n", 1);
        }
    }

    // collect exit status information

    do {
        status = wait();
        if (status == E_NO_PROCS) {
            break;
        }
        cwrite("User U: child exit status ");
        j = cvt_dec(buf, status);
        cwrites(buf, j);
        cwrites("\n", 1);
    } while (1);

    exit(EXIT_SUCCESS);
    return (0);  // shut the compiler up!

}


/*
** User W prints W characters 20 times, sleeping 3 seconds between.
*/

int32_t user_w(void) {
    int i;
    int32_t pid;
    char buf[12];

    pid = getpid();
    cwrites("User W (", 8);
    i = cvt_dec(buf, pid);
    cwrites(buf, i);
    cwrites(") running\n", 10);

    for (i = 0; i < 20; ++i) {
        swrites("W", 1);
        sleep(3);
    }

    cwrites("User W (", 8);
    cwrites(buf, i);
    cwrites(") exiting\n", 10);

    exit(EXIT_SUCCESS);
    return (0);  // shut the compiler up!

}


/*
** User X prints X characters 20 times.  It is spawned multiple
** times, prints its PID when started and exiting, and exits
** with a non-zero status
*/

int32_t user_x(void) {
    int i, j;
    int32_t pid;
    char buf[12];

    pid = getpid();
    cwrites("User X (", 8);
    i = cvt_dec(buf, pid);
    cwrites(buf, i);
    cwrites(") running\n", 10);

    for (i = 0; i < 20; ++i) {
        swrites("X", 1);
        for (j = 0; j < DELAY_STD; ++j)
            continue;
    }

    cwrites("User X (", 8);
    cwrites(buf, i);
    cwrites(") exiting\n", 10);
    exit(pid ? pid : -98765);

    return (0);  // shut the compiler up!

}


/*
** User Y prints Y characters 10 times.
*/

int32_t user_y(void) {
    int i, j;

    for (i = 0; i < 10; ++i) {
        swrites("Y", 1);
        for (j = 0; j < DELAY_ALT; ++j)
            continue;
        sleep(1);
    }

    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

}


/*
** User Z prints Z characters 10 times.  Before it exits, its parent
** may have exited; it reports this so that we can verify reparenting.
*/

int32_t user_z(void) {
    int i, j;
    int32_t pid, ppid;
    char buf[12];

    pid = getpid();
    ppid = getppid();

    cwrites("User Z (", 8);
    i = cvt_dec(buf, pid);
    cwrites(buf, i);
    cwrite(") running, parent ");
    i = cvt_dec(buf, ppid);
    cwrites(buf, i);
    cwrites("\n", 1);

    for (i = 0; i < 10; ++i) {
        swrites("Z", 1);
        for (j = 0; j < DELAY_STD; ++j)
            continue;
    }

    // get "new" parent PID
    ppid = getppid();

    cwrites("User Z (", 8);
    i = cvt_dec(buf, pid);
    cwrites(buf, i);
    cwrite(") exiting, parent now ");
    i = cvt_dec(buf, ppid);
    cwrites(buf, i);
    cwrites("\n", 1);

    exit(EXIT_SUCCESS);

    return (0);  // shut the compiler up!

    return (0);  // shut the compiler up!

}


/*
** SYSTEM PROCESSES
*/

/*
** Idle process
*/

int32_t idle(void) {
    int i;
    int32_t pid;
    uint32_t time;
    char buf[12];

    pid = getpid();
    time = gettime();
    cwrites("Idle (", 6);
    i = cvt_dec(buf, pid);
    cwrites(buf, i);
    cwrite(") started @ ");
    i = cvt_hex(buf, time);
    cwrites(buf, i);
    cwrites("\n", 1);

    swrites(".", 1);

    for (; ;) {
        for (i = 0; i < DELAY_LONG; ++i)
            continue;
        swrites(".", 1);
    }

    time = gettime();
    cwrite("+++ Idle done @ ");
    i = cvt_hex(buf, time);
    cwrites(buf, i);
    cwrite("!?!?!\n");

    exit(EXIT_FAILURE);
    return (0);  // shut the compiler up!

}


/*
** Initial process; it starts the other top-level user processes.
*/

int32_t init(void) {
    int pid, i;
    int32_t status;
    char buf[12];

    cwrites("Init started\n", 13);

    swrites("$", 1);

	pid = fork();
	if( pid < 0 ) {
		cwrite( "init, fork() user ME failed\n" );
	} else if( pid == 0 ) {
		exec( user_me );
		cwrite( "init, exec() user ME failed\n" );
		exit( EXIT_FAILURE );
	}
	swrites( "/a/", 3 );

#ifdef SPAWN_A
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user A failed\n");
    } else if (pid == 0) {
        exec(user_a);
        cwrite("init, exec() user A failed\n");
        exit(EXIT_FAILURE);
    }
    swrites("/a/", 3);
#endif

#ifdef SPAWN_B
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user B failed\n");
    } else if (pid == 0) {
        exec(user_b);
        cwrite("init, exec() user B failed\n");
        exit(EXIT_FAILURE);
    }
    swrites("/b/", 3);
#endif

#ifdef SPAWN_C
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user C failed\n");
    } else if (pid == 0) {
        exec(user_c);
        cwrite("init, exec() user C failed\n");
        exit(EXIT_FAILURE);
    }
    swrites("/c/", 3);
#endif

#ifdef SPAWN_D
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user D failed\n");
    } else if (pid == 0) {
        exec(user_d);
        cwrite("init, exec() user D failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_E
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user E failed\n");
    } else if (pid == 0) {
        exec(user_e);
        cwrite("init, exec() user E failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_F
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user F failed\n");
    } else if (pid == 0) {
        exec(user_f);
        cwrite("init, exec() user F failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_G
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user G failed\n");
    } else if (pid == 0) {
        exec(user_g);
        cwrite("init, exec() user G failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_H
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user H failed\n");
    } else if (pid == 0) {
        exec(user_h);
        cwrite("init, exec() user H failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_J
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user J failed\n");
    } else if (pid == 0) {
        exec(user_j);
        cwrite("init, exec() user J failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_K
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user K failed\n");
    } else if (pid == 0) {
        exec(user_k);
        cwrite("init, exec() user K failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_L
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user L failed\n");
    } else if (pid == 0) {
        exec(user_l);
        cwrite("init, exec() user L failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_M
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user M failed\n");
    } else if (pid == 0) {
        exec(user_m);
        cwrite("init, exec() user M failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_N
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user N failed\n");
    } else if (pid == 0) {
        exec(user_n);
        cwrite("init, exec() user N failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_P
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user P failed\n");
    } else if (pid == 0) {
        exec(user_p);
        cwrite("init, exec() user P failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_Q
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user Q failed\n");
    } else if (pid == 0) {
        exec(user_q);
        cwrite("init, exec() user Q failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_R
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user R failed\n");
    } else if (pid == 0) {
        exec(user_r);
        cwrite("init, exec() user R failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_S
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user S failed\n");
    } else if (pid == 0) {
        exec(user_s);
        cwrite("init, exec() user S failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_T
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user T failed\n");
    } else if (pid == 0) {
        exec(user_t);
        cwrite("init, exec() user T failed\n");
        exit(EXIT_FAILURE);
    }
#endif

#ifdef SPAWN_U
    pid = fork();
    if (pid < 0) {
        cwrite("init, fork() user U failed\n");
    } else if (pid == 0) {
        exec(user_u);
        cwrite("init, exec() user U failed\n");
        exit(EXIT_FAILURE);
    }
#endif

    swrites("!", 1);

    /*
    ** At this point, we go into an infinite loop waiting
    ** for our children (direct, or inherited) to exit.
    */

    for (; ;) {
        status = wait();
        cwrite("INIT: child exited, status ");
        i = cvt_dec(buf, status);
        cwrites(buf, i);
        cwrites("\n", 1);
    }

    /*
    ** SHOULD NEVER REACH HERE
    */

    cwrites("*** INIT IS EXITING???\n", 23);
    exit(EXIT_FAILURE);

    return (0);  // shut the compiler up!

}
