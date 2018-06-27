#include <stdio.h>

#include "VGA_BIND.H"
#include "INPUT_~1.H"

word *clock = (word *) 0x046C;

int main()
{
	BITMAP *bmp;
	MOUSE  mouse;
	MOUSEBITMAP * mb, * mouse_norm, * mouse_wait, * mouse_new = NULL;

	word redraw;
	sword dx = 0, dy = 0, new_x, new_y;
	word press, release;

	int i, j, done = 0, x, y;
	word last_time;

	mb = (MOUSEBITMAP *) malloc(sizeof(MOUSEBITMAP));
	mouse_norm = mb;
	mouse.bmp = mouse_norm;

	if (!init_mouse(&mouse))
	{
		printf("ERROR: Mouse not found. Quit.");
		exit(1);
	}

	bmp = load_bmp("MOUSE.BMP");
	set_mode(VGA_256_COLOR_MODE);

	for (y = 0; y < MOUSE_HEIGHT; y++)
		for (x = 0; x < MOUSE_WIDTH; x++)
			mb->data[x+y*MOUSE_WIDTH] = bmp->data[i*MOUSE_WIDTH+x+y*bmp->width];

	mb->hot_x = 7;
	mb->hot_y = 2;

	draw_bmp(bmp, 20, 20);
	draw_bmp(bmp, 50, 50);
	draw_bmp(bmp, 80, 80);

	free(bmp->data);

	set_palette(bmp->palette);

	new_x = mouse.x;
	new_y = mouse.y;

	redraw = 0xFFFF;

	show_mouse(&mouse);

	last_time = *clock;

	while (!done)
	{
		if (redraw)
		{
			wait_for_retrace();
			hide_mouse(&mouse);
			if (redraw > 1)
			{

			}
			if (mouse_new != NULL) mouse.bmp = mouse_new;
			mouse.x = new_x;
			mouse.y = new_y;
			show_mouse(&mouse);
			last_time = *clock;
			redraw = 0;
			mouse_new = NULL;
		}

		do {
			get_mouse_motion(&dx, &dy);
			press = get_mouse_press(LEFT_BUTTON);
			release = get_mouse_release(LEFT_BUTTON);
		} while (dx == 0 && dy == 0 && press == 0 && release == 0 && *clock == last_time);

		if (*clock != last_time)
		{

		}

		if (press) done = 1;

		if (dx || dy)
		{
			new_x = mouse.x + dx;
			new_y = mouse.y + dy;

			if (new_x < 0) new_x = 0;
			if (new_y < 0) new_y = 0;

			if (new_x > 319) new_x = 319;
			if (new_y > 199) new_y = 199;
			redraw = 1;
		}
	}

	free(mb);

	set_mode(TEXT_MODE);
	return 0;
}