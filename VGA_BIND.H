#ifndef __VGA_BIND_H_
#define __VGA_BIND_H_

#define VIDEO_INT 0x10          /* the BIOS video interrupt. */
#define WRITE_DOT 0x0C          /* BIOS func to plot a pixel. */
#define SET_MODE 0x00           /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE 0x13 /* use to set 256-color mode. */
#define TEXT_MODE 0x03          /* use to set 80x25 text mode. */

#define SCREEN_WIDTH 320  /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT 200 /* height in pixels of mode 0x13 */
#define NUM_COLORS 256    /* number of colors in mode 0x13 */

#define sign(x) ((x < 0) ? -1 : ((x > 0) ? 1 : 0))

typedef unsigned char byte;
typedef unsigned short word;
typedef long           fixed16_16;

fixed16_16 SIN_ACOS[1024];

byte far *VGA = (byte far *)0xA0000000L; /* this points to video memory. */
word *sys_clock = (word *)0x0000046C;    /* this points to the 18.2hz system
                                         clock. */

void init_sinacos_table()
{
    int i;
    for(i=0;i<1024;i++)                 /* create the sin(arccos(x)) table. */
    {
        SIN_ACOS[i]=sin(acos((float)i/1024))*0x10000L;
    }
}

/**************************************************************************
 *  set_mode                                                              *
 *     Sets the video mode.                                               *
 **************************************************************************/

void set_mode(byte mode)
{
    union REGS regs;

    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86(VIDEO_INT, &regs, &regs);
}

void plot_pixel(int x, int y, byte color)
{
    VGA[(y << 8) + (y << 6) + x] = color;
}

void plot_line(int x1, int y1, int x2, int y2, byte color)
{
    int i, dx, dy, sdx, sdy, x, y, px, py;

    dx = x2 - x1;
    dy = y2 - y1;

    sdx = sign(dx);
    sdy = sign(dy);

    dx = abs(dx);
    dy = abs(dy);

    px = x1;
    py = y1;

    VGA[(py << 8) + (py << 6) + px] = color;

    if (dx >= dy)
    {
        for (i = 0; i < dx; i++)
        {
            y += dy;
            if (y >= dx)
            {
                y -= dx;
                py += sdy;
            }
            px += sdx;
            plot_pixel(px, py, color);
        }
    }
    else
    {
        for (i = 0; i < dy; i++)
        {
            x += dx;
            if (x >= dy)
            {
                x -= dy;
                px += sdx;
            }
            py += sdy;
            plot_pixel(px, py, color);
        }
    }
}

void draw_polygon(int num_verts, int *verts, byte color)
{
    int i;
    for (i = 0; i < num_verts - 1; i++)
    {
        plot_line(
            verts[(i<<1)+0],
            verts[(i<<1)+1],
            verts[(i<<1)+2],
            verts[(i<<1)+3],
            color
        );
    }
    plot_line(
        verts[0],
        verts[1],
        verts[(num_verts<<1)-2],
        verts[(num_verts<<1)-1],
        color
    );
}

void draw_rect(int left, int top, int right, int bottom, byte color)
{
    word top_offset, bottom_offset, i, temp;

    if (top > bottom)
    {
        temp = top;
        top = bottom;
        bottom = temp;
    }
    if (left > right)
    {
        temp = left;
        left = right;
        right = temp;
    }

    top_offset = (top<<8) + (top<<6);
    bottom_offset = (bottom<<8) + (bottom<<6);

    for (i = left; i <= right; i++)
    {
        VGA[top_offset+i] = color;
        VGA[bottom_offset+i] = color;
    }
    for (i = top_offset; i<= bottom_offset; i+= SCREEN_WIDTH)
    {
        VGA[left+i] = color;
        VGA[right+i] = color;
    }
}

void rect_fill(int left, int top, int right, int bottom, byte color)
{
    word top_offset, bottom_offset, i, temp, width;

    if (top > bottom)
    {
        temp = top;
        top = bottom;
        bottom = temp;
    }
    if (left > right)
    {
        temp = left;
        left = right;
        right = temp;
    }

    top_offset = (top<<8) + (top<<6);
    bottom_offset = (bottom<<8) + (bottom<<6);

    width = right - left + 1;

    for (i = top_offset; i <= bottom_offset; i += SCREEN_WIDTH)
    {
        memset(&VGA[i], color, width);
    }
}

void draw_circle(int x, int y, int radius, byte color)
{
    fixed16_16 n = 0, invradius=(1/(float) radius) * 0x10000L;
    int dx = 0, dy = radius -1;
    word dxoffset, dyoffset, offset = (y<<8) + (y<<6) +x;

    while (dx <= dy)
    {
        dxoffset = (dx<<8) + (dx<<6);
        dyoffset = (dy<<8) + (dy<<6);

        VGA[offset + dy - dxoffset] = color; /* octant 0 */
        VGA[offset + dx - dyoffset] = color; /* octant 1 */
        VGA[offset - dx - dyoffset] = color; /* octant 2 */
        VGA[offset - dy - dxoffset] = color; /* octant 3 */
        VGA[offset - dy + dxoffset] = color; /* octant 4 */
        VGA[offset - dx + dyoffset] = color; /* octant 5 */
        VGA[offset + dx + dyoffset] = color; /* octant 6 */
        VGA[offset + dy + dxoffset] = color; /* octant 7 */
        dx++;
        n += invradius;
        dy = (int)((radius * SIN_ACOS[(int)(n>>6)]) >> 16);
    }
}

#endif