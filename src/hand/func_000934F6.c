#include "records.h"
/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000934F6 */
#include "structs.h"
#include "clib.h"
extern iptr screen_buffer;
extern char str_inven_c[];
extern char str_fmt_d[];
extern char str_done[];
extern char str_days[];
extern signed char text_buffer[];
extern struct record *scratch_current_object;
extern struct record *inv_right_container;
extern iptr color_remap;
extern int game_minutes;
extern int trade_mode;
extern unsigned char game_mode_under_popup;
extern signed char game_mode;
extern iptr inv_background_image;
extern struct record *inv_selected_item;
extern struct record *inv_left_container;
extern unsigned char *color_remap_tables;

extern void text_draw_coloured(char *, short, short, int, unsigned char);
extern void inv_draw_item_image(char *, struct rect *, short);
extern void inv_draw_cart_cell(char *, struct rect *, int);
extern void inv_draw_cell_mark(int, int, struct rect *, int);
#pragma aux mc_set_location parm routine [];

#define SCREEN (*(char **)&screen_buffer)
#define BUFS ((char **)&inv_background_image)
#define MODE ((unsigned char)game_mode)
#define MODE2 (game_mode_under_popup)
#define U16(p, o) (*(unsigned short *)((p) + (o)))

int inv_draw_item_cell(char *object, short cell, struct rect *rects)
{
    short unused1;
    int unused2;
    int unused3;
    short unused4;
    short unused5;
    int image_bump;
    int days;
    char *item;
    short unused6;
    short y;

    image_bump = 0;
    *(char * *)&scratch_current_object = object;
    item = RECORD_DATA(object);
    if (object == (char *)inv_selected_item && MODE != 10 && MODE2 != 10) {
        for (y = rects[cell].y0; y <= rects[cell].y1; y++)
            mc_memcpy(SCREEN + y * 320 + rects[cell].x0, (char *)(iptr)BUFS[0] + y * 320 + rects[cell].x0,
                          rects[cell].x1 - rects[cell].x0 + 1, str_inven_c, 681, 4);
    }
    if (*(unsigned char *)object == 54) {
        for (y = rects[cell].y0; y <= rects[cell].y1; y++)
            mc_memcpy(SCREEN + y * 320 + rects[cell].x0, (char *)(iptr)BUFS[1] + y * 320 + rects[cell].x0,
                          rects[cell].x1 - rects[cell].x0 + 1, str_inven_c, 687, 4);
    }
    if (object[38] != 0) {
        for (y = rects[cell].y0; y <= rects[cell].y1; y++)
            mc_memcpy(SCREEN + y * 320 + rects[cell].x0, (char *)(iptr)BUFS[2] + y * 320 + rects[cell].x0,
                          rects[cell].x1 - rects[cell].x0 + 1, str_inven_c, 693, 4);
    }
    if (object != (char *)inv_left_container && object != (char *)inv_right_container && *(short *)(item + 67) != -1
        && MODE != 10 && MODE2 != 10)
        inv_draw_cell_mark(380, 5, rects, cell);
    if ((U16(item, 42) & 64) && MODE != 10 && MODE2 != 10)
        inv_draw_cell_mark(380, 7, rects, cell);
    if (U16(object, 21) & 32)
        inv_draw_cell_mark(380, 6, rects, cell);
    *(char **)&color_remap = (char *)color_remap_tables + (*(unsigned char *)(item + 56) << 8);
    if (U16(item, 32) == 3 && U16(item, 34) == 8) {
        image_bump++;
        (*(short *)(item + 50))++;
    }
    if (U16(item, 32) == 23 && U16(item, 34) == 0)
        inv_draw_cart_cell(item, rects, cell);
    else
        inv_draw_item_image(item, rects, cell);
    *(short *)(item + 50) -= image_bump;
    *(char **)&color_remap = (char *)color_remap_tables;
    if (U16(item, 32) == 3 && U16(item, 34) == 18) {
        mc_set_location(723, str_inven_c);
        mc_sprintf(((char *)text_buffer), str_fmt_d, *(unsigned char *)(item + 49));
        text_draw_coloured(((char *)text_buffer), rects[cell].x0 + 3, rects[cell].y0 + 2, 145, 156);
    }
    if (trade_mode == 3 && *(unsigned char *)object == 54) {
        if ((unsigned)game_minutes >= *(unsigned *)(object + 43)) {
            *(short *)(item + 44) = *(short *)(item + 46);
            mc_strncpy(((char *)text_buffer), str_done, 160, str_inven_c, 732);
        } else {
            days = (*(unsigned *)(object + 43) - (unsigned)game_minutes) / 1440;
            if (days == 0) days++;
            mc_set_location(738, str_inven_c);
            mc_sprintf(((char *)text_buffer), str_days, days);
        }
        text_draw_coloured(((char *)text_buffer), rects[cell].x0 + 3, rects[cell].y0 + 2, 145, 156);
    }
    return 1;
}
