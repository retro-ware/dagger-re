/* sheet.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */

#include "records.h"
#include "bitfield.h"
#include "clib.h"

extern char str_info00i0_img[];
extern char str_popu00i0_img[];
extern char str_cust09i0_img_3[];
extern char str_sheet_c[];
extern signed char world_running;
extern char scratch_190be4[];
extern char scratch_190d64[];
extern short levelup_attribute_floor[];
extern signed char player_view_flags;
extern struct record *player_object;
extern struct image *screen_image_1;
extern struct character *player_character;
extern iptr window_image;
extern struct career *player_class;
extern int player_death_timer;
extern short list_popup_height;
extern short list_popup_y;
extern short list_popup_x;
extern short list_popup_width;
extern signed char sheet_selected_attribute;
extern unsigned char game_mode_under_popup;
extern signed char screen_active;
extern signed char game_mode;
extern struct image *sheet_popup_image;

extern int key_action_held(int);
extern int sound_play(int, struct record *, int);
extern iptr disk_read_file(char *, iptr);
extern int rand_range(int, int);
extern void sheet_place_spinner(int);

int sheet_open(short opening)
{
    int hp_gain;
    int result;

    if (player_death_timer > 0) return 0;
    if (((int)game_mode_under_popup) == 3 && ((int)(unsigned char)game_mode) == 8) {
        return 1;
    }
    if (opening != 0 || (game_mode == 0 && key_action_held(36) != 0 && player_death_timer == 0)) {
        game_mode = 3;
        window_image = disk_read_file(str_info00i0_img, 0);
        sheet_popup_image = (struct image *)disk_read_file(str_popu00i0_img, 0);
        screen_image_1 = (struct image *)disk_read_file(str_cust09i0_img_3, 0);
        screen_active = 1;
        list_popup_x = 160 - (sheet_popup_image->width >> 1);
        list_popup_y = 100 - (sheet_popup_image->height >> 1);
        list_popup_width = sheet_popup_image->width;
        list_popup_height = sheet_popup_image->height;
        if (((struct bf8_2_1 *)&player_view_flags)->f != 0) {
            hp_gain = (rand_range(player_class->hp_per_level >> 1, player_class->hp_per_level) + (player_character->attributes[4] / 10)) - 5;
            if (hp_gain < 1) hp_gain = 1;
            player_character->max_health += hp_gain;
            player_character->max_health_base += hp_gain;
            sound_play(364, player_object, 100);
        }
        if (((int)(short)opening) == 50) {
            player_view_flags |= 4;
            *(short *)scratch_190d64 = 30;
        } else {
            *(short *)scratch_190d64 = rand_range(4, 6);
        }
        *(int *)scratch_190be4 = 0;
        mc_memcpy(levelup_attribute_floor, (signed char *)&player_character->base_attributes[0], 16, str_sheet_c, 97, 4);
        sheet_place_spinner(13);
        sheet_selected_attribute = 0;
        world_running = 0;
    }
    if (((int)(unsigned char)game_mode) == 3) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}
