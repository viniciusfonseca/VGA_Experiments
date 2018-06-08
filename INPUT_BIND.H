#ifndef __INPUT_BIND
#define __INPUT_BIND

#include "SIZDEFS.H"

#define MOUSE_INT         0x33
#define MOUSE_RESET       0x00
#define MOUSE_GETPRESS    0x05
#define MOUSE_GETRELEASE  0x06
#define MOUSE_GETMOTION   0x0B
#define LEFT_BUTTON       0x00
#define RIGHT_BUTTON      0x01
#define MIDDLE_BUTTON     0x02

#define CURSOR_WIDTH      24
#define CURSOR_HEIGHT     24
#define CURSOR_SIZE       (CURSOR_WIDTH * CURSOR_HEIGHT)

#define STATE_NORMAL      0
#define STATE_ACTIVE      1
#define STATE_PRESSED     2
#define STATE_WAITING     3

#define NUM_BUTTONS       2
#define NUM_MOUSEBITMAPS  9

typedef short sword;

typedef struct tagMOUSEBITMAP
{
    int hot_x;
    int hot_y;
    byte data[CURSOR_SIZE];
    MOUSEBITMAP * next;
} MOUSEBITMAP;

typedef struct
{
    byte on;
    byte button1;
    byte button2;
    byte button3;
    int num_buttons;
    sword x;
    sword y;
    byte under[CURSOR_SIZE];
    MOUSEBITMAP * bmp;
} MOUSE;

sword init_mouse(MOUSE * mouse)
{
    union REGS regs;
    sword dx, dy;
    regs.x.ax = MOUSE_RESET;
    int86(MOUSE_INT, &regs, &regs);

    mouse->on = regs.x.ax;
    mouse->num_buttons = regs.x.bx;
    mouse->button1 = 0;
    mouse->button2 = 0;
    mouse->button3 = 0;
    mouse->x = SCREEN_WIDTH / 2;
    mouse->y = SCREEN_HEIGHT / 2;
    get_mouse_motion(&dx, &dy);

    return mouse->on;
}

void get_mouse_motion(sword *dx, sword *dy)
{
    union REGS regs;

    regs.x.ax = MOUSE_GETMOTION;
    int86(MOUSE_INT, &regs, &regs);
    *dx = regs.x.cx;
    *dy = regs.x.dx;
}

#endif