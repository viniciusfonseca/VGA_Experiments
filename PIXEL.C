#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <math.h>
#include "VGA_BIND.H"

/**************************************************************************
 *  Main                                                                  *
 *    Plots 50000 pixels two different ways: using the BIOS and by        *
 *    directly writing to video memory.                                   *
 **************************************************************************/

void main()
{
  set_mode(VGA_256_COLOR_MODE);

  draw_rect(20, 20, SCREEN_WIDTH-20, SCREEN_HEIGHT-20, 0xF);
  draw_rect(19, 19, SCREEN_WIDTH-19, SCREEN_HEIGHT-19, 0xF);

  draw_rect(16, 16, SCREEN_WIDTH-16, SCREEN_HEIGHT-16, 0xF);

  // memset(VGA + 643, 0xF, 3);
  byte far * f = &VGA[640];
  *f = 0xF;
  *(f+1) = 0xF;
  *(f+2) = 0xF;

  getchar();
  
  set_mode(TEXT_MODE);

  return;
}