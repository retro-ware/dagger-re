/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0009C29C */
#include "records.h"

extern unsigned char mouse_buttons;
extern unsigned char world_running;
extern unsigned char travel_selected_region;
extern int travel_selected_location;
extern unsigned char scratch_190ce5;
extern struct record *player_object;
extern struct character *player_character;
extern unsigned char mouse_buttons_prev;
extern unsigned char time_skip_active;
extern unsigned char quests_suspended;
extern struct map_location *region_locations;
extern int travel_dest_x;
extern int travel_dest_z;
extern int sound_play(int, struct record *, int);
extern void location_place_player_at_edge(unsigned int);
extern void map_goto_location(int, int, int, int);
extern void travel_button_exit(int);
extern void travel_close_region(void);
extern int travel_route(int, int, int, int, int);
extern int xn_math_angle_to_point(int, int, int, int);

void travel_teleport_yes(void)
{
    int unused;
    int minutes;
    int saved_fatigue;
    int direction;

    direction = (((xn_math_angle_to_point(player_object->x, player_object->z, region_locations[travel_selected_location].x_type_flags & 0x1ffffff, region_locations[travel_selected_location].z_size & 0xffffff) >> 2) + 32) & 511) >> 6;
    if ((mouse_buttons & 1) == 0 || (mouse_buttons_prev & 1) != 0)
        return;
    sound_play(203, player_object, 110);
    scratch_190ce5 = 0;
    travel_close_region();
    quests_suspended = 1;
    time_skip_active = 1;
    world_running = 1;
    saved_fatigue = player_character->fatigue;
    minutes = travel_route(player_object->x, player_object->z, travel_dest_x, travel_dest_z, 1);
    travel_close_region();
    travel_button_exit(100);
    if (minutes != -1)
        map_goto_location(travel_selected_region, 1, travel_selected_location, 0);
    if (minutes != -1)
        location_place_player_at_edge(direction);
    quests_suspended = 0;
    time_skip_active = 0;
}
