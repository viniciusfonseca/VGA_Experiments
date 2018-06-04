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
    sword dx, dy;
    MOUSE * mouse = (MOUSE *) malloc(sizeof(MOUSE));
    union REGS regs;
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

void interrupt get_scan()
{
    asm cli
    asm {
        IN  AL, 060h    /* read scan code */
        MOV gb_scan, AL
        IN  AL, 061h    /* read keyboard status */
        MOV BL, AL
        OR  AL, 080h
        OUT 061h, AL    /* set bit 7 and write */
        MOV AL, BL
        OUT 061h, AL    /* write again, bit 7 clear */

        MOV AL, 020h    /* reset PIC */
        OUT 020h, AL

        STI
    }
}

#endif