/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0008227A */
#include "records.h"

struct flags {
    unsigned char b0:1;
    unsigned char b1:1;
    unsigned char b2:1;
    unsigned char b3:1;
    unsigned char b4:1;
    unsigned char b5:1;
};
extern struct collide_probe player_collide_probe;
extern struct collide_probe player_probe_climb;
extern struct collide_probe player_probe_crouched;
extern struct collide_probe player_probe_riding;
extern struct move_request player_move_request;
extern struct flags player_motion_flags;
extern struct record *player_object;
extern struct character *player_character;
extern unsigned char collide_flags;
extern int collide_move_player(struct record *, int, struct move_request *, int);

int player_try_move_vertical(int dy)
{
    int result;
    int unused;

    player_move_request.x = player_object->x;
    player_move_request.y = player_object->y + dy;
    player_move_request.z = player_object->z;
    player_move_request.angle_x = player_object->angle_x;
    player_move_request.yaw = player_object->yaw;
    player_move_request.angle_z = player_object->angle_z;
    player_move_request.probe = player_motion_flags.b2 ? &player_probe_crouched : &player_collide_probe;
    player_move_request.probe = (player_character->flags & 1536) ? &player_probe_riding : player_move_request.probe;
    if (player_motion_flags.b5)
        player_move_request.probe = &player_probe_climb;
    collide_flags &= 251;
    result = collide_move_player(player_object, 0, &player_move_request, 1);
    return result;
}
