/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00038911 */
#include "records.h"
#include "bitfield.h"
#include "clib.h"

#pragma pack(1)
extern unsigned char mouse_buttons;
extern short mouse_x;
extern short mouse_y;
extern struct spell *selected_spell;
extern unsigned char spell_effect_school[];
extern unsigned char spell_effect_cost_formula[];
extern struct rect spellmaker_setting_buttons[34];
extern unsigned char magic_school_skills[];
extern signed char text_buffer[];
extern unsigned char ui_mode_flags;
extern struct character *player_character;
extern short spell_effect_slot;
extern short D_00195F34;
extern short saved_mouse_y;
extern short saved_mouse_x;
extern short mouse_motion_y;
extern unsigned char msgbox_kind;
extern char game_mode;
extern unsigned char mouse_buttons_prev;
extern iptr spellmaker_settings_image;
extern short spellmaker_drag_accum;
extern short spellmaker_drag_row;
extern unsigned char spellmaker_settings_kind;
extern int spell_effect_text_index(short);
extern void msgbox_open_rsc(int, int);
extern void msgbox_update(void);
extern void text_draw_coloured(char *, short, short, int, unsigned char);
extern void text_draw_number_in_box(short, short, short, short, short, short, short);
extern int spell_cost_formula_dispatch(int);
extern void xn_draw_fullscreen_overlay_shaded(char *);
extern void xn_mouse_set_position(int, int);

void spellmaker_settings_update(void)
{
    int val;
    int r;
    short i;

    if (spellmaker_settings_kind == 0)
        return;
    if (msgbox_kind != 4) {
        msgbox_open_rsc(spell_effect_text_index(spell_effect_slot) + 1500, 4);
        game_mode = 2;
        ui_mode_flags |= 64;
        D_00195F34 = 88;
    }
    if ((int)(unsigned char)(mouse_buttons & 1) != 0 && mouse_buttons_prev != mouse_buttons && mouse_x > 281
      && mouse_x < 304 && mouse_y > 94 && mouse_y < 109) {
        ui_mode_flags |= 32;
        msgbox_update();
        spellmaker_settings_kind = 0;
        return;
    }
    xn_draw_fullscreen_overlay_shaded((char *)spellmaker_settings_image);
    msgbox_update();
    if ((int)(unsigned char)(mouse_buttons & 1) != 0 && mouse_buttons != mouse_buttons_prev) {
        for (i = 11; i < 33; i++) {
            if (mouse_x > spellmaker_setting_buttons[i].x0 && mouse_x < spellmaker_setting_buttons[i].x1
              && mouse_y > spellmaker_setting_buttons[i].y0 && mouse_y < spellmaker_setting_buttons[i].y1) {
                if (i < 22)
                    mouse_motion_y = 1;
                else
                    mouse_motion_y = -1;
                r = i % 11;
                if (r < 3 && (int)(unsigned char)(spellmaker_settings_kind & 1) != 0
                  || r < 6 && (int)(unsigned char)(spellmaker_settings_kind & 2) != 0
                  || (int)(unsigned char)(spellmaker_settings_kind & 4) != 0)
                    spellmaker_setting_buttons[i % 11].handler(mouse_motion_y);
                goto done;
            }
        }
        mouse_motion_y = i = 0;
        for (; i < 11; i++) {
            if (mouse_x > spellmaker_setting_buttons[i].x0 && mouse_x < spellmaker_setting_buttons[i].x1
              && mouse_y > spellmaker_setting_buttons[i].y0 && mouse_y < spellmaker_setting_buttons[i].y1)
                mouse_motion_y = i + 1;
        }
        if (mouse_motion_y == 0)
            goto done;
        spellmaker_drag_row = mouse_motion_y - 1;
        ((struct bf8_2_1 *)&ui_mode_flags)->f = 1;
        saved_mouse_x = mouse_x;
        saved_mouse_y = mouse_y;
        spellmaker_drag_accum = 0;
    } else if (mouse_buttons == 0 && ((struct bf8_2_1 *)&ui_mode_flags)->f) {
        ((struct bf8_2_1 *)&ui_mode_flags)->f = 0;
        xn_mouse_set_position(saved_mouse_x, saved_mouse_y);
    } else if (((struct bf8_2_1 *)&ui_mode_flags)->f) {
        spellmaker_drag_accum -= mouse_motion_y;
        if (abs(spellmaker_drag_accum) > 30) {
            if (spellmaker_drag_row < 3 && (int)(unsigned char)(spellmaker_settings_kind & 1) != 0
              || spellmaker_drag_row < 6 && (int)(unsigned char)(spellmaker_settings_kind & 2) != 0
              || (int)(unsigned char)(spellmaker_settings_kind & 4) != 0)
                spellmaker_setting_buttons[spellmaker_drag_row].handler(spellmaker_drag_accum / 30);
            spellmaker_drag_accum = 0;
        }
    }
done:
    val = selected_spell->effect_costs[spell_effect_slot];
    val = (110 - player_character->skills[magic_school_skills[spell_effect_school[selected_spell->effects[spell_effect_slot].type]]].value) * val / 100;
    text_draw_coloured(itoa(val, ((char *)text_buffer), 10), 275, 119, 145, 156);
    if ((int)(unsigned char)(spellmaker_settings_kind & 1) != 0) {
        text_draw_number_in_box(64, 94, 87, 109, selected_spell->durations[spell_effect_slot].base, 145, 156);
        text_draw_number_in_box(104, 94, 127, 109, selected_spell->durations[spell_effect_slot].plus, 145, 156);
        text_draw_number_in_box(160, 94, 183, 109, selected_spell->durations[spell_effect_slot].per_level, 145, 156);
    }
    if ((int)(unsigned char)(spellmaker_settings_kind & 2) != 0) {
        text_draw_number_in_box(64, 114, 87, 129, selected_spell->chances[spell_effect_slot].base, 145, 156);
        text_draw_number_in_box(104, 114, 127, 129, selected_spell->chances[spell_effect_slot].plus, 145, 156);
        text_draw_number_in_box(160, 114, 183, 129, selected_spell->chances[spell_effect_slot].per_level, 145, 156);
    }
    if ((int)(unsigned char)(spellmaker_settings_kind & 4) != 0) {
        text_draw_number_in_box(64, 134, 87, 149, selected_spell->magnitudes[spell_effect_slot].base_min, 145, 156);
        text_draw_number_in_box(104, 134, 127, 149, selected_spell->magnitudes[spell_effect_slot].base_max, 145, 156);
        text_draw_number_in_box(144, 134, 167, 149, selected_spell->magnitudes[spell_effect_slot].plus_min, 145, 156);
        text_draw_number_in_box(184, 134, 207, 149, selected_spell->magnitudes[spell_effect_slot].plus_max, 145, 156);
        text_draw_number_in_box(240, 134, 263, 149, selected_spell->magnitudes[spell_effect_slot].per_level, 145, 156);
    }
    selected_spell->effect_costs[spell_effect_slot] = spell_cost_formula_dispatch(spell_effect_cost_formula[selected_spell->effects[spell_effect_slot].type] - 1);
}
