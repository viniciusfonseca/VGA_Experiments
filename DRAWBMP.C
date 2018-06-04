#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <math.h>
#include "VGA_BIND.H"

int main()
{
    BITMAP * bmp = load_bmp("DISTEA~1.BMP");
    
    printf("Successful read bmp.");
    getchar();

    set_mode(VGA_256_COLOR_MODE);

    draw_bmp(bmp, 0, 0);

    getchar();
    set_mode(TEXT_MODE);

    return 0;
}