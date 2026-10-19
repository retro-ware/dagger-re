/* text.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "ptrint.h"

extern signed char msgbox_border_sizes[];
extern signed char msgbox_border_style_tiles[];
extern iptr msgbox_spop_tiles[];
extern iptr msgbox_border_tiles;
extern short msgbox_tile_size;
extern short msgbox_tile_h;
extern short msgbox_tile_w;


void msgbox_set_border_style(short style)
{
    msgbox_border_tiles = msgbox_spop_tiles[style];
    msgbox_tile_w = (unsigned short)(unsigned char)msgbox_border_sizes[style * 2];
    msgbox_tile_h = (unsigned short)(unsigned char)msgbox_border_style_tiles[style * 2];
    msgbox_tile_size = msgbox_tile_w * msgbox_tile_h;
}
