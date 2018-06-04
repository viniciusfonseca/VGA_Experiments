#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include <mem.h>
#include "VGA_BIND.H"
                         
/* debug purpose only */
void dump_bmp(BITMAP * bmp)
{
    int c, i, j, size=bmp->width*bmp->height;
    printf("DUMP\n");
    c=0;
    for (i = 0; i < size; i++)
    {
        printf("%02X ", bmp->data[i]);
        c++;
        if (c == 16)
        {
            printf("\n");
            c=0;
        }
    }
}

int main()
{
    int size;
    BITMAP * bmp = load_bmp("DISTEA~1.BMP");
    
    size=bmp->width*bmp->height;
    printf("Successful read bmp. Size: %dx%d (%d bytes)",
        bmp->width, bmp->height,
        size);                         

    getchar();

    set_mode(VGA_256_COLOR_MODE);
    
    draw_bmp(bmp, 4, 60);

    getchar();
    set_mode(TEXT_MODE);

    return 0;
}