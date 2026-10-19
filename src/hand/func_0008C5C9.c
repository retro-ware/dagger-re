/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0008C5C9 */
#include "ptrint.h"
#include "clib.h"
#include "doslow.h"
extern unsigned char text_colour;
extern short font_height;
extern short pen_x;
extern short pen_y;
extern char str_inpstr_c[];
extern char inpstr_saved_text[];
extern int inpstr_result;
extern iptr inpstr_text;
extern short text_cursor_x;
extern short text_cursor_y;
extern short inpstr_cursor;
extern void text_draw(iptr, unsigned short, unsigned short);
extern int inpstr_read_key(void);
extern int inpstr_handle_key(unsigned char);
extern short inpstr_text_width(iptr, short);
extern void xn_draw_line(int, int, int, int);

int inpstr_update(void)
{
    unsigned char c;
    char x;
    char y;

    text_colour += 5;
    pen_x = inpstr_text_width(inpstr_text, inpstr_cursor) + text_cursor_x;
    pen_y = text_cursor_y;
    {
        int *clk;

        clk = (int *)DOS_LOW(0x46C);
        if (*clk & 4)
            xn_draw_line(pen_x, pen_y, pen_x, (short)(pen_y + font_height - 1));
    }
    text_colour -= 5;
    text_draw(inpstr_text, text_cursor_x, text_cursor_y);
    c = inpstr_read_key();
    if (c == 0)
        return 0;
    {
        int r;

        r = inpstr_handle_key(c);
        if (r == 0x87654321)
            return 0;
        if (r == 0x8000) {
            mc_strncpy((char *)inpstr_text, inpstr_saved_text, 4, str_inpstr_c, 157);
            return 2;
        }
        inpstr_result = r;
        return 1;
    }
}
