/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000537B6 */
#include "records.h"
#include "clib.h"

extern char text_colour;
extern iptr screen_buffer;
extern char str_custom_c[];
extern struct rect classmaker_reputation_buttons[];
extern signed char text_buffer[];
extern iptr scratch_190df4;
extern char *scratch_190df8;
extern struct character *player_character;
extern void msgbox_update(void);
extern void text_draw_centred_coloured(char *, short, short, int, unsigned char);
extern void xn_mouse_cursor_erase(void);
extern void xn_mouse_cursor_draw(void);
extern void xn_draw_fill_rect(short, short, short, short);
extern void xn_draw_image(int, int, int, int, char *);

void classmaker_draw_reputations(void)
{
    short i;
    short width;
    short centre;
    short total;
    short height;

    xn_mouse_cursor_erase();
    mc_memcpy((void *)screen_buffer, (void *)scratch_190df4, 64000, str_custom_c, 283, 4);
    xn_draw_image(39, 5, *(unsigned short *)(scratch_190df8 + 4), *(unsigned short *)(scratch_190df8 + 6), scratch_190df8 + 12);
    width = classmaker_reputation_buttons[0].x1 - classmaker_reputation_buttons[0].x0 + 1;
    centre = (classmaker_reputation_buttons[0].y0 + classmaker_reputation_buttons[0].y1) >> 1;
    total = i = 0;
    for (; i < 5; i++) {
        height = player_character->reputation[i] * 5;
        if (player_character->reputation[i] < 0) {
            text_colour = 0xf6;
            xn_draw_fill_rect(classmaker_reputation_buttons[i].x0, 82, width, -height);
        } else if (player_character->reputation[i] > 0) {
            text_colour = 0xc5;
            xn_draw_fill_rect(classmaker_reputation_buttons[i].x0, 81 - height, width, height);
        }
        text_draw_centred_coloured(itoa(player_character->reputation[i], ((char *)text_buffer), 10), i * 33 + 58, 149, 145, 141);
        total += player_character->reputation[i];
    }
    text_draw_centred_coloured(itoa(-total, ((char *)text_buffer), 10), 105, 179, 145, 141);
    msgbox_update();
    xn_mouse_cursor_draw();
}
