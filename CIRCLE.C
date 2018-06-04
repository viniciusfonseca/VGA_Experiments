#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <math.h>
#include "VGA_BIND.H"

int main()
{
    int radius;

    init_sinacos_table();
    set_mode(VGA_256_COLOR_MODE);

    for (radius=48;radius>0;radius-=3)
    {
        draw_circle(50, 50, radius, radius);    
    }

    getchar();
    set_mode(TEXT_MODE);

    return 0;
}