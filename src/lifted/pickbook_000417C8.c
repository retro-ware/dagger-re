/* pickbook.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"

extern iptr screen_buffer;
extern char str_pickbook_c[];
extern char str_spbk00i0_img[];
extern char str_mask04i0_img_2[];
extern iptr D_00184876;
extern signed char world_running;
extern signed char D_001940D4;
extern signed char ui_dirty_flags;
extern struct record *player_object;
extern iptr magic_window_image;
extern iptr window_image;
extern int spell_cast_busy;
extern int player_death_timer;
extern unsigned char game_mode_under_popup;
extern signed char screen_active;
extern signed char game_mode;
extern iptr spellbook_saved_screen;

extern int spellbook_build_list(void);
extern int key_action_held(int);
extern int sound_play(int, struct record *, int);
extern iptr disk_read_file(char *, iptr);
extern int xn_font_select(int);
extern void hud_status_set(iptr);

int spellbook_open(short force)
{
    int is_open;

    if (((int)game_mode_under_popup) == 5 && ((int)(unsigned char)game_mode) == 8) {
        return 1;
    }
    if (player_death_timer > 0) return 0;
    if (force != 0 || (game_mode == 0 && key_action_held(28) != 0)) {
        if (spell_cast_busy != 0) {
            hud_status_set(D_00184876);
            return 0;
        }
        xn_font_select(4);
        ui_dirty_flags &= 254;
        if (spellbook_build_list() == 0) return 0;
        D_001940D4 |= 128;
        world_running = 0;
        ui_dirty_flags |= 2;
        spellbook_saved_screen = (iptr)mc_malloc(64000, str_pickbook_c, 101);
        mc_memcpy((void *)spellbook_saved_screen, (void *)screen_buffer, 64000, str_pickbook_c, 102, 4);
        game_mode = 5;
        window_image = disk_read_file(str_spbk00i0_img, 0);
        magic_window_image = disk_read_file(str_mask04i0_img_2, 0);
        screen_active = 1;
        sound_play(237, player_object, 100);
    }
    if (((int)(unsigned char)game_mode) == 5) {
        is_open = 1;
    } else {
        is_open = 0;
    }
    return is_open;
}
