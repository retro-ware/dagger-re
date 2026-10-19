/* intrface.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "bitfield.h"
#include "doslow.h"

extern signed char mouse_buttons;
extern short mouse_x;
extern short mouse_y;
extern int steer_turn_speed_max;
extern short player_speed;
extern struct character *player_character;
extern signed char mouse_control_mode;
extern short player_base_speed;
extern signed char player_moving;
extern int steer_start_tick;
extern char turn_this_frame[];
extern int steer_ticks;
extern int move_angle_offset;
extern short steer_weight_down;
extern short steer_weight_right;
extern short steer_region_width;
extern short steer_region_height;
extern short steer_weight_left;
extern short steer_weight_up;
extern short steer_key_region;

extern void xn_input_steer_dispatch(int);

void intrface_steer(int unused, int region, int region_x, int region_y)
{
    int *bios_ticks;
    {
        unsigned char moving;

        if (((int)(unsigned char)(mouse_buttons & 1)) != 0 || ((int)(short)steer_key_region) != (-1)) {
            if (((int)(short)steer_key_region) == (-1)) {
                steer_ticks = steer_turn_speed_max;
                steer_weight_right = ((((int)(short)mouse_x) - region_x) << 8) / ((int)(short)steer_region_width);
                steer_weight_down = ((((int)(short)mouse_y) - region_y) << 8) / ((int)(short)steer_region_height);
                steer_weight_left = ((((int)(short)steer_region_width) - (((int)(short)mouse_x) - region_x)) << 8) / ((int)(short)steer_region_width);
                steer_weight_up = ((((int)(short)steer_region_height) - (((int)(short)mouse_y) - region_y)) << 8) / ((int)(short)steer_region_height);
            } else {
                bios_ticks = (int *)DOS_LOW(0x46C);
                if ((steer_ticks = *bios_ticks - steer_start_tick) > steer_turn_speed_max) {
                    steer_ticks = steer_turn_speed_max;
                }
            }
        }
        move_angle_offset = 0;
        player_speed = player_base_speed;
        if ((player_character->conditions & 0x1) == 0 && ((((int)(unsigned char)mouse_control_mode) != 1 && ((int)(unsigned char)(mouse_buttons & 1)) != 0) || ((int)(short)steer_key_region) != (-1))) {
            xn_input_steer_dispatch(region);
        } else {
            player_speed = 0;
            *(int *)turn_this_frame = 0;
            move_angle_offset = 0;
        }
        if (((int)(short)player_speed) > 2) {
            moving = 1;
        } else {
            moving = 0;
        }
        player_moving = moving;
    }
}
