/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000820AF */
#include "records.h"

#pragma pack(1)
struct bits8 { unsigned char b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1; };
#pragma pack()
extern struct collide_probe D_00187B6E;
extern struct collide_probe player_probe_climb;
extern struct collide_probe player_probe_crouched;
extern struct collide_probe player_probe_riding;
extern struct move_request player_move_request;
extern struct bits8 player_motion_flags;
extern struct record *player_object;
extern struct character *player_character;
extern short collide_flags;
extern int move_remainder_z;
extern int move_remainder_x;
extern int move_angle_offset;
extern int collide_move_player(struct record *, int, struct move_request *, int);
extern void xn_math_yaw_offset_xz(int, int, int *, int *);

int player_try_move(int distance)
{
    int dx;
    int dz;
    int result;

    xn_math_yaw_offset_xz((player_object->yaw + move_angle_offset) & 2047, distance << 5, &dx, &dz);
    dx += player_object->x << 5;
    dz += player_object->z << 5;
    dx += move_remainder_x;
    dz += move_remainder_z;
    move_remainder_x = dx & 31;
    move_remainder_z = dz & 31;
    player_move_request.x = dx / 32;
    player_move_request.y = player_object->y;
    player_move_request.z = dz / 32;
    player_move_request.angle_x = player_object->angle_x;
    player_move_request.yaw = player_object->yaw;
    player_move_request.angle_z = player_object->angle_z;
    if (player_move_request.x < 16384 || player_move_request.x > 32751616 || player_move_request.z < 16384 || player_move_request.z > 16367616) {
        collide_flags |= 8;
        return collide_flags;
    }
    player_move_request.probe = player_motion_flags.b2 ? &player_probe_crouched : &D_00187B6E;
    player_move_request.probe = ((unsigned short)player_character->flags & 1536) != 0 ? &player_probe_riding : player_move_request.probe;
    if (player_motion_flags.b5)
        player_move_request.probe = &player_probe_climb;
    if (distance != 0)
        collide_flags |= 4;
    else
        collide_flags &= ~4;
    result = collide_move_player(player_object, 0, &player_move_request, 1);
    return result;
}
