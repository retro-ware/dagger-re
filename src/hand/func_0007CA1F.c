/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007CA1F */
#include "ptrint.h"
extern unsigned char text_shadow_colour;
extern unsigned char text_colour;
extern void text_draw_shadow(char *, slot16, slot16);

void text_draw_coloured(char *text, short x, int y, int colour, int shadow)
{
    short old_colour;
    short old_shadow;

    old_colour = text_colour;
    old_shadow = text_shadow_colour;
    text_colour = colour;
    text_shadow_colour = shadow;
    text_draw_shadow(text, x, y);
    text_colour = old_colour;
    text_shadow_colour = old_shadow;
}
