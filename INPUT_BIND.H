#ifndef __INPUT_BIND
#define __INPUT_BIND

#include "SIZDEFS.H"
#include "VGA_BIND.H"
#include <DOS.H>

#define MOUSE_INT         0x33
#define MOUSE_RESET       0x00
#define MOUSE_GETPRESS    0x05
#define MOUSE_GETRELEASE  0x06
#define MOUSE_GETMOTION   0x0B
#define LEFT_BUTTON       0x00
#define RIGHT_BUTTON      0x01
#define MIDDLE_BUTTON     0x02

#define MOUSE_WIDTH      24
#define MOUSE_HEIGHT     14
#define MOUSE_SIZE       (MOUSE_WIDTH * MOUSE_HEIGHT)

#define STATE_NORMAL      0
#define STATE_ACTIVE      1
#define STATE_PRESSED     2
#define STATE_WAITING     3

#define NUM_BUTTONS       2
#define NUM_MOUSEBITMAPS  9

typedef struct tagMOUSEBITMAP
{
    int hot_x;
    int hot_y;
    byte data[MOUSE_SIZE];
    struct tagMOUSEBITMAP * next;
} MOUSEBITMAP;

typedef struct tagMOUSE
{
    byte on;
    byte button1;
    byte button2;
    byte button3;
    int num_buttons;
    sword x;
    sword y;
    byte under[MOUSE_SIZE];
    MOUSEBITMAP * bmp;
} MOUSE;

void get_mouse_motion(sword *, sword *);

sword init_mouse(MOUSE * mouse)
{
    sword dx, dy, on, num_buttons;
    asm mov ax, MOUSE_RESET
    asm int     MOUSE_INT
    asm mov on, ax
    mouse->on = on;
    asm mov num_buttons, bx
    mouse->num_buttons = num_buttons;
    mouse->button1 = 0;
    mouse->button2 = 0;
    mouse->button3 = 0;
    mouse->x = SCREEN_WIDTH / 2;
    mouse->y = SCREEN_HEIGHT / 2;
    get_mouse_motion(&dx, &dy);

    return mouse->on;
}

void get_mouse_motion(sword * dx, sword * dy)
{
    sword _cx, _dx;
    asm mov ax, MOUSE_GETMOTION
    asm int     MOUSE_INT
    asm mov _cx, cx
    asm mov _dx, dx
    *dx = _cx;
    *dy = _dx;
}

sword get_mouse_press(sword button)
{
	sword _bx;
	asm mov ax, MOUSE_GETPRESS
	asm mov bx, button
	asm int     MOUSE_INT
	asm mov _bx, bx
	return _bx;
}

sword get_mouse_release(sword button)
{
	sword _bx;
	asm mov ax, MOUSE_GETRELEASE
	asm mov bx, button
	asm int     MOUSE_INT
	asm mov _bx, bx
	return _bx;
}

void show_mouse(MOUSE * mouse)
{
	int x, y;
	int mx = mouse->x - mouse->bmp->hot_x;
	int my = mouse->y - mouse->bmp->hot_y;
	long screen_offset = (my << 8) + (my << 6);
	word bitmap_offset = 0;
	byte data;

	for (y = 0; y < MOUSE_HEIGHT; y++)
	{
		for (x = 0; x < MOUSE_WIDTH; x++, bitmap_offset++)
		{
			mouse->under[bitmap_offset] = VGA[(word)(screen_offset+mx+x)];
			if (mx + x < SCREEN_WIDTH  && mx + x >= 0 &&
			    my + y < SCREEN_HEIGHT && my + y >= 0)
			    data = mouse->bmp->data[bitmap_offset];
			    if (data) VGA[(word)(screen_offset+mx+x)] = data;
		}
		screen_offset += SCREEN_WIDTH;
	}
}

void hide_mouse(MOUSE * mouse)
{
	int x, y;
	int mx = mouse->x - mouse->bmp->hot_x;
	int my = mouse->y - mouse->bmp->hot_y;
	long screen_offset = (my << 8) + (my << 6);
	word bitmap_offset = 0;

	for (y = 0; y < MOUSE_HEIGHT; y++)
	{
		for (x = 0; x < MOUSE_WIDTH; x++, bitmap_offset++)
		{
			if (mx + x < SCREEN_WIDTH  && mx + x >= 0 &&
			    my + y < SCREEN_HEIGHT && my + y >= 0)
				VGA[(word)(screen_offset + mx + x)] = mouse->under[bitmap_offset];
		}
		screen_offset += SCREEN_WIDTH;
	}
}



#endif