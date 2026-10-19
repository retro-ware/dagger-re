/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00072E85 */
#include "records.h"
#include "bitfield.h"

extern int xn_anim_ticks;
extern char str_you_have_no_arrows[];
extern short cast_anim_state;
extern char bow_anim_script[];
extern signed char D_001940D6;
extern signed char misc_state_flags;
extern signed char player_motion_flags;
extern int right_hand_ready_delay[];
extern struct record *player_entity;
extern struct record *player_object;
extern int frame_ticks;
extern struct record *spell_ready_missile;
extern struct image *hud_bar_image;
extern iptr color_remap;
extern struct character *player_character;
extern struct settings *game_settings;
extern signed char weapon_active_hand;
extern signed char screen_active;
extern iptr weapon_hand_item[];
extern int weapon_hand_group[];
extern int weapon_hand_tick[];
extern int weapon_hand_timer[];
extern int weapon_timer_2;
extern iptr weapon_hand_cif[];
extern int weapon_hand_frame[];
extern iptr weapon_hand_anim[];
extern int weapon_swing_state[];
extern unsigned char *color_remap_tables;
extern void fatigue_add(int);
extern int weapon_start_swing(int);
extern void weapon_bow_update(void);
extern void weapon_melee_strike(struct record *);
extern void hud_status_set(char *);
extern int inv_take_arrow(int);
extern void *xn_img_cif_group(void *, int);
extern void xn_draw_cif_rle_frame(void *, int, int, int);
extern void xn_draw_img_masked_remap(void *, int);

void weapon_player_update(void)
{
    int bar_height;
    iptr cif;
    int unused;

    if (right_hand_ready_delay[((int)(unsigned char)weapon_active_hand)] != 0) return;
    if ((player_character->conditions & 0x1) != 0) return;
    if (cast_anim_state >= 0 || (iptr)spell_ready_missile != 0 || screen_active != 0 || ((struct bf8_6_1 *)&misc_state_flags)->f != 0) {
        return;
    }
    if (weapon_hand_cif[((int)(unsigned char)weapon_active_hand)] == 0) return;
    if (((struct bf8_5_1 *)&player_motion_flags)->f != 0) return;
    if (((struct bf8_6_1 *)&D_001940D6)->f == 0) return;
    if (((iptr)bow_anim_script) == *(iptr *)&weapon_hand_anim[((int)(unsigned char)weapon_active_hand)]) {
        if (inv_take_arrow(0) == 0) {
            hud_status_set(str_you_have_no_arrows);
            D_001940D6 &= 191;
            return;
        }
        weapon_bow_update();
        return;
    }
    if (weapon_hand_item[((int)(unsigned char)weapon_active_hand)] != 0) {
        /* the item's colour: its data's +56 (the record's header first) */
        color_remap = (iptr)((char *)color_remap_tables + (((int)(unsigned char)*(signed char *)((char *)weapon_hand_item[((int)(unsigned char)weapon_active_hand)] + RECORD_HEADER_SIZE + 56)) << 8));
    }
    if (((struct bf8_6_1 *)&D_001940D6)->f != 0 && weapon_swing_state[((int)(unsigned char)weapon_active_hand)] == 0) {
        cif = weapon_hand_cif[((int)(unsigned char)weapon_active_hand)];
        if (((int)(unsigned short)(game_settings->view_flags & 1)) != 0) {
            bar_height = 0;
        } else {
            bar_height = hud_bar_image->height;
        }
        xn_draw_img_masked_remap((void *)cif, -bar_height);
    }
    if (weapon_swing_state[((int)(unsigned char)weapon_active_hand)] == 0) {
        weapon_hand_timer[((int)(unsigned char)weapon_active_hand)] = 1;
        if (weapon_hand_timer[((int)(unsigned char)weapon_active_hand)] < 0) {
            weapon_hand_timer[((int)(unsigned char)weapon_active_hand)] = 0;
        }
        weapon_start_swing((int)(unsigned char)weapon_active_hand);
        color_remap = (iptr)color_remap_tables;
        return;
    }
    if (weapon_hand_timer[0] > 0) weapon_hand_timer[0] -= frame_ticks;
    if (weapon_timer_2 > 0) weapon_timer_2 -= frame_ticks;
    if (*(iptr *)&weapon_hand_anim[((int)(unsigned char)weapon_active_hand)] != 0) {
        xn_draw_cif_rle_frame(xn_img_cif_group((void *)weapon_hand_cif[((int)(unsigned char)weapon_active_hand)], weapon_hand_group[((int)(unsigned char)weapon_active_hand)]), (int)(unsigned char)*(signed char *)((char *)(*(iptr *)&weapon_hand_anim[((int)(unsigned char)weapon_active_hand)] + weapon_hand_frame[((int)(unsigned char)weapon_active_hand)])), ((((int)(unsigned short)(game_settings->view_flags & 1)) != 0) ? 0 : -hud_bar_image->height), (int)(unsigned char)weapon_active_hand);
    } else {
        xn_draw_cif_rle_frame(xn_img_cif_group((void *)weapon_hand_cif[((int)(unsigned char)weapon_active_hand)], weapon_hand_group[((int)(unsigned char)weapon_active_hand)]), 5 - weapon_swing_state[((int)(unsigned char)weapon_active_hand)], ((((int)(unsigned short)(game_settings->view_flags & 1)) != 0) ? 0 : -hud_bar_image->height), (int)(unsigned char)weapon_active_hand);
    }
    if (weapon_hand_timer[((int)(unsigned char)weapon_active_hand)] <= 0) {
        weapon_hand_timer[((int)(unsigned char)weapon_active_hand)] = (115 - player_character->attributes[6]) * 3;
        weapon_hand_tick[((int)(unsigned char)weapon_active_hand)] = xn_anim_ticks;
        (weapon_swing_state[((int)(unsigned char)weapon_active_hand)])--;
        (weapon_hand_frame[((int)(unsigned char)weapon_active_hand)])++;
        if (*(iptr *)&weapon_hand_anim[((int)(unsigned char)weapon_active_hand)] != 0) {
            if (((int)(unsigned char)*(signed char *)((*(char * *)&weapon_hand_anim[((int)(unsigned char)weapon_active_hand)] + weapon_hand_frame[((int)(unsigned char)weapon_active_hand)]))) == 255) {
                weapon_swing_state[((int)(unsigned char)weapon_active_hand)] = 0;
            } else {
                weapon_swing_state[((int)(unsigned char)weapon_active_hand)] = 2;
            }
        }
        if ((*(iptr *)&weapon_hand_anim[((int)(unsigned char)weapon_active_hand)] != 0 && ((int)(unsigned char)*(signed char *)((*(char * *)&weapon_hand_anim[((int)(unsigned char)weapon_active_hand)] + weapon_hand_frame[((int)(unsigned char)weapon_active_hand)]))) == 4) || (*(iptr *)&weapon_hand_anim[((int)(unsigned char)weapon_active_hand)] == 0 && weapon_swing_state[((int)(unsigned char)weapon_active_hand)] == 3)) {
            player_entity->x = player_object->x;
            player_entity->y = player_object->y;
            player_entity->z = player_object->z;
            player_entity->angle_x = player_object->angle_x;
            player_entity->yaw = player_object->yaw;
            player_entity->angle_z = player_object->angle_z;
            weapon_melee_strike(player_entity);
            fatigue_add(-11);
        }
    }
    color_remap = (iptr)color_remap_tables;
}
