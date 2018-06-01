#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>

#include "VGA_BIND.H"

void main()
{
    int i, j, c = -1;
    set_mode(VGA_256_COLOR_MODE);

    for (i = 0; i < 4; i++)
    {
		if (c >= 256) break;
		for (j = 0; j < 80; j++)
		{
			c++;
			if (c >= 256) break;

			plot_pixel(j*2  , i*2  , c);
			plot_pixel(j*2+1, i*2  , c);
			plot_pixel(j*2  , i*2+1, c);
			plot_pixel(j*2+1, i*2+1, c);
		}
    }

    getchar();
    set_mode(TEXT_MODE);
}