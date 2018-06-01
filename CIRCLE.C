#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <math.h>
#include "VGA_BIND.H"

int main()
{
    init_sinacos_table();
    set_mode(VGA_256_COLOR_MODE);

    int r;
    for (r=48;r>0;r-=3)
    {
        draw_circle(50, 50, r, r);    
    }

    getchar();
    set_mode(TEXT_MODE);

    return 0;
}