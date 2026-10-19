/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0008C2D7 */
extern unsigned char text_colour;
extern short font_height;
extern short pen_x;
extern short pen_y;
extern char str_inpstr_c[];        /* __FILE__ */
extern char inpstr_saved_text[];
extern char *inpstr_text;
extern short inpstr_max_length;
extern short inpstr_cursor;
extern void text_draw(char *, short, short);
extern int inpstr_read_key(void);
extern int inpstr_handle_key(unsigned char);
extern int inpstr_text_width(char *, short);
#include "clib.h"
#include "doslow.h"
extern void xn_gfx_wait_vretrace_start(void);
extern void xn_gfx_wait_vretrace_end(void);
extern void xn_gfx_present_inclusive(int);
extern void xn_mouse_cursor_erase(void);
extern void xn_kbd_flush(void);
extern void xn_draw_fill_rect(short, short, short, short);
extern void xn_draw_line(short, short, short, short);

int inpstr_edit(char *text, short x, short y, short w, short h, short max_length)
{
    short key;
    unsigned char old;
    int r;

    xn_mouse_cursor_erase();
    xn_kbd_flush();
    inpstr_text = text;
    mc_strncpy(inpstr_saved_text, inpstr_text, 160, str_inpstr_c, 56);
    inpstr_cursor = strlen(inpstr_text);
    inpstr_max_length = max_length;
    for (;;) {
        key = inpstr_read_key();
        if (key == 0) {
            old = text_colour;
            text_colour = 0;
            xn_draw_fill_rect(x, y, w, h);
            text_colour = 12;
            pen_x = x + inpstr_text_width(inpstr_text, inpstr_cursor);
            pen_y = y;
            if (*(int *)DOS_LOW(0x46C) & 32)
                xn_draw_line(pen_x, pen_y, pen_x, pen_y + font_height - 1);
            text_colour = old;
            text_draw(inpstr_text, x, y);
            xn_gfx_present_inclusive(1);
            xn_gfx_wait_vretrace_start();
            xn_gfx_wait_vretrace_end();
            continue;
        }
        r = inpstr_handle_key(key);
        if (r == 32768) {
            mc_strncpy(inpstr_text, inpstr_saved_text, 4, str_inpstr_c, 83);
            return 0;
        }
        if (r != 0x87654321)
            return r;
    }
}
