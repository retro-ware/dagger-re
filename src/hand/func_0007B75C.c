/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007B75C */
#include "records.h"
#include "clib.h"

extern signed char text_colour;
extern short D_00142928;
extern short D_0014292C;
extern iptr screen_buffer;
extern iptr big_buffer;
extern char str_loadsave_c[];
extern struct rect saveload_buttons[];
extern struct image *screen_image_1;
extern iptr window_image;
extern char *scratch_buffer;
extern void text_draw_centred_coloured(iptr, int, int, int, unsigned char);
extern void xn_draw_image(int, int, int, int, char *);
extern void xn_draw_line_to(int, int);

void saveload_draw(int saving, int used_slots, int slot)
{
    int i;

    mc_memcpy((void *)screen_buffer, (void *)window_image, 64000, str_loadsave_c, 977, 4);
    if (saving == 0) goto L7B7D9;
    xn_draw_image(screen_image_1->x, screen_image_1->y, screen_image_1->width, screen_image_1->height, screen_image_1->pixels);
L7B7D9:;
    for (i = 0; i < 6; i++) {
        if (((1 << i) & used_slots) != 0) {
            xn_draw_image((((i) < 3) ? 40 : 200), ((i % 3) * 65) + 4, 80, 50, (*(char **)&big_buffer + (i * 4000)));
            text_draw_centred_coloured((iptr)(scratch_buffer + (i << 5)), (int)(short)(((i) < 3) ? 80 : 246), (int)(short)(((i % 3) * 65) + 57), 145, 156);
        }
    }

    i = ((slot < 3) ? slot : slot + 3);
    text_colour = 146;
    D_00142928 = saveload_buttons[i].x0 - 1;
    D_0014292C = saveload_buttons[i].y0 - 1;
    xn_draw_line_to((int)(short)(saveload_buttons[i].x1 + 1), (int)(short)(saveload_buttons[i].y0 - 1));
    xn_draw_line_to((int)(short)(saveload_buttons[i].x1 + 1), (int)(short)(saveload_buttons[i].y1 + 1));
    xn_draw_line_to((int)(short)(saveload_buttons[i].x0 - 1), (int)(short)(saveload_buttons[i].y1 + 1));
    xn_draw_line_to((int)(short)(saveload_buttons[i].x0 - 1), (int)(short)(saveload_buttons[i].y0 - 1));
}
