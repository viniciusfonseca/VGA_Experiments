#include <dos.h>
#include <stdlib.h>

int main()
{
    const char * msg = "Hello x86 Assembly";

    asm mov ax, 0x03 // save next video mode (80x25, 16 color text)
    asm int     0x10 // call interrupt to video mode

    asm mov dx, &msg // save string address
    asm mov cx, 0x12 // save string length
    asm mov bx, 0x01 // set output device to standard (stdout)

    asm mov ah, 0x40 // set DOS subroutine to write device / file
    asm int     0x21 // call DOS subroutine

    asm mov ah, 0x08 // set DOS subroutine to read console input
    asm int     0x21 // call DOS subroutine

    return 0;
}