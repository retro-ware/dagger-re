/* text.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "ptrint.h"
#include "clib.h"

extern signed char mouse_buttons;
extern iptr screen_buffer;
extern char str_text_c[];
extern short msgbox_wrap_width;
extern signed char msgbox_kind;
extern unsigned char popup_answer;
extern signed char screen_active;
extern signed char game_mode;
extern signed char mouse_buttons_prev;
extern char msgbox_image[];
extern char *msgbox_saved_screen;

extern char *text_expand_wrap(unsigned short, short, char *, char *, char *);
extern void xn_mouse_poll_clamped(void);
extern int xn_font_select(int);
extern void msgbox_render(char *, iptr);
extern void msgbox_wait(void);
extern void mode_push(void);

void msgbox_show_string(char *text, short kind)
{
    int flags;
    int length;
    char *copy;
    char *wrap_buf;
    char *expand_buf;

    if (msgbox_kind != 0) return;
    msgbox_saved_screen = mc_malloc(64000, str_text_c, 697);
    mc_memcpy(msgbox_saved_screen, (void *)screen_buffer, 64000, str_text_c, 698, 4);
    xn_font_select(4);
    length = strlen(text);
    copy = mc_malloc(length + 16, str_text_c, 702);
    wrap_buf = mc_malloc(((length < 4096) ? 8192 : length * 2), str_text_c, 703);
    expand_buf = mc_malloc(((length < 4096) ? 8192 : length * 2), str_text_c, 704);
    mc_strncpy(copy, text, 4, str_text_c, 705);
    if (kind == 5) {
        flags = 4;
        popup_answer = 0;
    } else {
        flags = 0;
    }
    copy = text_expand_wrap((int)(unsigned short)(flags | 32770), (int)(short)msgbox_wrap_width, copy, wrap_buf, expand_buf);
    msgbox_render(copy, (iptr)msgbox_image);
    if (copy != 0 && copy != (char *)(iptr)-1751672937) {
        mc_free(copy, str_text_c, 717);
        copy = (char *)(iptr)-1751672937;
    }
    msgbox_kind = kind;
    mode_push();
    game_mode = 8;
    screen_active = 1;
    mouse_buttons_prev = mouse_buttons;
    xn_mouse_poll_clamped();
    msgbox_wait();
}
