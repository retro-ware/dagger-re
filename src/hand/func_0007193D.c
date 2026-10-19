/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007193D */
#include "records.h"
#include "clib.h"
#include "doslow.h"

extern signed char mouse_buttons;
extern short mouse_x;
extern short mouse_y;
extern signed char text_colour;
extern signed char key_down_esc;
extern signed char key_down_enter;
extern char str_your_time_for_this_room_has_expired[];
extern char str_rest_c[];
extern char str_rest02i0_img[];
extern char rest_prompt_fmt[];
extern char str_rest01i0_img[];
extern char loiter_prompt_fmt[];
extern iptr text_rest_how_many;
extern iptr text_loiter_how_many;
extern signed char text_buffer[];
extern int rest_last_tick;
extern signed char rest_state;
extern short rest_hours_left;
extern struct record *player_entity;
extern int creature_count;
extern char inpstr_result[];
extern struct character *player_character;
extern struct career *player_class;
extern char *scratch_buffer;
extern unsigned char game_mode_under_popup;
extern signed char game_mode;
extern signed char mouse_buttons_prev;
extern signed char time_skip_active;
extern signed char encounter_spawned;
extern signed char rest_active;
extern int rest_ticks_per_hour;
extern struct image *rest_image;
extern signed char rest_loitering;
extern void msgbox_show_string(iptr, int);
extern void msgbox_show_rsc(int, int);
extern void time_pass_minutes(int);
extern void hud_draw(void);
extern iptr disk_read_file(char *, iptr);
extern void rest_recover(struct record *);
extern void rest_close(void);
extern int rest_allowed(void);
extern int rest_room_expired(void);
extern void text_draw_centred_coloured(iptr, int, int, int, unsigned char);
extern void inpstr_begin_number(int);
extern int point_in_rect(short, short, short, short, short, short);
extern int xn_mouse_poll_clamped(void);
extern void xn_gfx_clear(int);
extern void xn_draw_image(int, int, int, int, char *);
#pragma aux mc_set_location parm routine [];

void rest_update(void)
{
    int max_hours;
    iptr prompt;


    rest_active = 0;
    if (((int)(unsigned char)game_mode) != 16) {
        if (((int)game_mode_under_popup) != 16 || ((int)(unsigned char)game_mode) != 8) {
            return;
        }
    }
    xn_gfx_clear(0);
    hud_draw();
    rest_active = 1;
    text_colour = 146;
    if (rest_room_expired() != 0) {
        msgbox_show_string((iptr)str_your_time_for_this_room_has_expired, 1);
        rest_close();
        return;
    }
    if (creature_count != 0 || encounter_spawned != 0) {
        msgbox_show_rsc(354, 1);
        rest_close();
        return;
    }
    if (key_down_esc != 0) {
        key_down_esc = 0;
        rest_close();
        return;
    }
    if (rest_state == 0) {
        xn_draw_image(rest_image->x, rest_image->y, rest_image->width, rest_image->height, rest_image->pixels);
        if (mouse_buttons != 0 && mouse_buttons_prev == 0 && point_in_rect((int)(short)mouse_x, (int)(short)mouse_y, 87, 63, 134, 86) != 0) {
            while (mouse_buttons != 0) xn_mouse_poll_clamped();
            if (rest_allowed() == 0) {
                rest_close();
                return;
            }
            rest_ticks_per_hour = 9;
            rest_state = 1;
            if ((iptr)rest_image != 0 && (iptr)rest_image != (-1751672937)) {
                mc_free(rest_image, str_rest_c, 165);
                rest_image = (struct image *)(iptr)-1751672937;
            }
            rest_image = (struct image *)disk_read_file(str_rest02i0_img, 0);
            prompt = (iptr)(scratch_buffer + 55000);
            mc_set_location(168, str_rest_c);
            mc_sprintf((char *)prompt, rest_prompt_fmt, text_rest_how_many);
            *(signed char *)((char *)(strlen((char *)prompt) + prompt) + 1) = 0;
            inpstr_begin_number(0);
            msgbox_show_string(prompt, 2);
            while (key_down_enter != 0);
        } else if (mouse_buttons != 0 && mouse_buttons_prev == 0 && point_in_rect((int)(short)mouse_x, (int)(short)mouse_y, 136, 63, 183, 86) != 0) {
            while (mouse_buttons != 0) xn_mouse_poll_clamped();
            if (rest_allowed() == 0) {
                rest_close();
                return;
            }
            rest_ticks_per_hour = 9;
            rest_state = 3;
            if ((iptr)rest_image != 0 && (iptr)rest_image != (-1751672937)) {
                mc_free(rest_image, str_rest_c, 184);
                rest_image = (struct image *)(iptr)-1751672937;
            }
            rest_image = (struct image *)disk_read_file(str_rest01i0_img, 0);
            rest_last_tick = *(int *)DOS_LOW(0x46C);
        } else if (mouse_buttons != 0 && mouse_buttons_prev == 0 && point_in_rect((int)(short)mouse_x, (int)(short)mouse_y, 185, 63, 232, 86) != 0) {
            while (mouse_buttons != 0) xn_mouse_poll_clamped();
            rest_ticks_per_hour = 32;
            rest_state = 1;
            if ((iptr)rest_image != 0 && (iptr)rest_image != (-1751672937)) {
                mc_free(rest_image, str_rest_c, 193);
                rest_image = (struct image *)(iptr)-1751672937;
            }
            rest_image = (struct image *)disk_read_file(str_rest02i0_img, 0);
            prompt = (iptr)(scratch_buffer + 55000);
            mc_set_location(196, str_rest_c);
            mc_sprintf((char *)prompt, loiter_prompt_fmt, text_loiter_how_many);
            *(signed char *)((char *)(strlen((char *)prompt) + prompt) + 1) = 0;
            inpstr_begin_number(0);
            msgbox_show_string(prompt, 2);
            rest_loitering = 1;
            time_skip_active = 1;
            while (key_down_enter != 0);
        }
        return;
    }
    if (((int)(signed char)rest_state) == 1) {
        if (((int)(unsigned char)game_mode) != 8) {
            if (rest_loitering != 0) {
                max_hours = 3;
            } else {
                max_hours = 99;
            }
            if (*(int *)inpstr_result > max_hours) {
                key_down_enter = 0;
                msgbox_show_rsc((int)(short)((rest_loitering != 0) ? 27 : 26), 1);
                rest_close();
            } else {
                rest_hours_left = *(short *)inpstr_result;
                rest_state = 2;
                rest_last_tick = *(int *)DOS_LOW(0x46C);
            }
        }
        return;
    }
    if (((int)(signed char)rest_state) == 2) {
        xn_draw_image(rest_image->x, rest_image->y, rest_image->width, rest_image->height, rest_image->pixels);
        text_draw_centred_coloured((iptr)itoa((int)(short)rest_hours_left, (char *)text_buffer, 10), 118, 62, 146, 156);
        if (mouse_buttons != 0 && mouse_buttons_prev == 0 && point_in_rect((int)(short)mouse_x, (int)(short)mouse_y, 140, 76, 179, 85) != 0) {
            rest_close();
        }
        if (((unsigned)(*(int *)DOS_LOW(0x46C) - rest_last_tick)) > rest_ticks_per_hour) {
            time_pass_minutes(60);
            if (rest_loitering == 0) rest_recover(player_entity);
            rest_hours_left--;
            rest_last_tick = *(int *)DOS_LOW(0x46C);
        }
        if (rest_hours_left == 0) {
            msgbox_show_rsc((int)(short)((rest_loitering != 0) ? 349 : 353), 1);
            rest_close();
        }
        return;
    }
    if (((int)(signed char)rest_state) != 3) return;
    xn_draw_image(rest_image->x, rest_image->y, rest_image->width, rest_image->height, rest_image->pixels);
    text_draw_centred_coloured((iptr)itoa((int)(short)rest_hours_left, (char *)text_buffer, 10), 118, 62, 146, 156);
    if (mouse_buttons != 0 && mouse_buttons_prev == 0 && point_in_rect((int)(short)mouse_x, (int)(short)mouse_y, 140, 76, 179, 85) != 0) {
        rest_close();
    } else if (((unsigned)(*(int *)DOS_LOW(0x46C) - rest_last_tick)) > rest_ticks_per_hour) {
        time_pass_minutes(60);
        rest_recover(player_entity);
        rest_hours_left++;
        rest_last_tick = *(int *)DOS_LOW(0x46C);
    }
    if (player_character->health != player_character->max_health || (player_character->magicka != player_character->max_magicka && (player_class->flags & 8) == 0) || player_character->fatigue != ((player_character->attributes[0] + player_character->attributes[4]) << 6)) {
        return;
    }
    msgbox_show_rsc(350, 1);
    rest_close();
}
