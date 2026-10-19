/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000234EB */
#include "records.h"
#include "bitfield.h"
#include "clib.h"

extern unsigned char player_environment;
extern struct collide_probe collide_object_probe;
extern unsigned char world_state_flags;
extern signed char player_motion_flags;
extern struct record *player_object;
extern struct character *player_character;
extern struct record *moving_model;
extern struct record *floor_object;
extern iptr collide_side_hit;
extern iptr collide_floor_hit;
extern short collide_wall_cos;
extern signed char player_on_ground;
extern signed char collide_landed;
extern int collide_candidate_count;
extern struct collide_probe *xn_collide_probe;
extern struct collide_hits *xn_slide_planes;
extern char collide_flags[];
extern int collide_step_player(struct record *, int, struct move_request *);
extern void collide_for_each_nearby(struct record *, iptr);
extern void collide_gather_cb(struct record *);
extern void automap_mark_seen(iptr);
extern iptr links_object_motion(iptr);
extern void xn_vec_normalize_ptr(void *);



#define FLAGS (*(short *)collide_flags)


int collide_move_player(struct record *object, int unused_arg, struct move_request *request, int unused_arg2)
{
    int result;
    int unused1;           /* unused1-3 are never read: they only shape the frame */
    int unused2;
    struct vec3 direction;
    int unused3;
    int dx;
    int dy;
    int dz;
    int best;
    int i;
    int mindot;
    int dot;
    int flags;
    short *motion;

    unused3 = 0;
    *(iptr *)&floor_object = *(iptr *)&moving_model = 0;
    collide_wall_cos = 10000;
    collide_floor_hit = collide_side_hit = 0;
    xn_slide_planes = 0;
    collide_candidate_count = 0;
    (xn_collide_probe = &collide_object_probe)->position.x = request->x;
    xn_collide_probe->position.y = request->y;
    xn_collide_probe->position.z = request->z;
    collide_for_each_nearby(object, (iptr)collide_gather_cb);
    if (!((player_character->conditions & 0x8) || collide_candidate_count != 0 || player_environment == 1))
        return FLAGS = 16;
    flags = FLAGS;
    player_on_ground = 1;
    result = collide_step_player(object, unused_arg, request);
    world_state_flags &= 223;
    if ((char)player_on_ground != 0 && (char)collide_landed != 0)
        collide_landed = 0;
    if ((iptr)floor_object != 0 && (motion = (short *)links_object_motion((iptr)floor_object)) != 0) {
        if (motion[0] != 0 || motion[2] != 0) {
            request->x += motion[0];
            request->y += motion[1];
            request->z += motion[2];
            *(unsigned char *)collide_flags |= 4;
            result = collide_step_player(object, unused_arg, request);
        }
    }
    if ((iptr)moving_model != 0 && (motion = (short *)links_object_motion((iptr)moving_model)) != 0) {
        request->x += motion[0];
        request->y += motion[1];
        request->z += motion[2];
        *(unsigned char *)collide_flags |= 4;
        result = collide_step_player(object, unused_arg, request);
    }
    if ((iptr)floor_object != 0)
        automap_mark_seen((iptr)floor_object);
    if ((iptr)moving_model != 0)
        automap_mark_seen((iptr)moving_model);
    if (result & 2)
        collide_wall_cos = 0;
    if (object == player_object && ((struct bf8_5_1 *)&player_motion_flags)->f)
        return 0;
    if (!(result & 10) || !(flags & 4))
        return 0;
    direction.x = request->x - object->x;
    direction.y = request->y - object->y;
    direction.z = request->z - object->z;
    xn_vec_normalize_ptr(&direction);
    if (xn_slide_planes == 0)
        return 1;
    if (xn_slide_planes->count > 1) {
        best = 0;
        mindot = 1000000;
        for (i = 0; i < xn_slide_planes->count; i++) {
            dot = direction.x * xn_slide_planes->hits[i].nx + direction.z * xn_slide_planes->hits[i].nz;
            if (dot < mindot) {
                mindot = dot;
                best = i;
            }
        }
    } else {
        mindot = direction.x * xn_slide_planes->hits[0].nx + direction.z * xn_slide_planes->hits[0].nz;
        best = 0;
    }
    dx = (request->x - object->x) * xn_slide_planes->hits[best].nx;
    dy = (request->y - object->y) * xn_slide_planes->hits[best].ny;
    dz = (request->z - object->z) * xn_slide_planes->hits[best].nz;
    dx = dz + (dx + dy);
    dy = dx * xn_slide_planes->hits[best].ny;
    dz = dx * xn_slide_planes->hits[best].nz;
    dx = dx * xn_slide_planes->hits[best].nx;
    dx >>= 8;
    dy >>= 8;
    dz >>= 8;
    dx = (request->x - object->x) - (dx >> 8);
    dy = (request->y - object->y) - (dy >> 8);
    dz = (request->z - object->z) - (dz >> 8);
    request->x = object->x + dx;
    request->y = object->y + dy;
    request->z = object->z + dz;
    collide_wall_cos = abs(mindot >> 16);
    if (dx != 0 || dy != 0 || dz != 0) {
        collide_floor_hit = collide_side_hit = 0;
        flags = FLAGS;
        player_on_ground = 1;
        result = collide_step_player(object, unused_arg, request);
        world_state_flags &= 223;
        if ((char)player_on_ground != 0 && (char)collide_landed != 0)
            collide_landed = 0;
        if ((iptr)floor_object != 0)
            automap_mark_seen((iptr)floor_object);
        if ((iptr)moving_model != 0)
            automap_mark_seen((iptr)moving_model);
        if (object == player_object && ((struct bf8_4_1 *)&player_motion_flags)->f)
            return 0;
        if (!(result & 10) || !(flags & 4))
            return 0;
        return 1;
    }
    return FLAGS;
}
