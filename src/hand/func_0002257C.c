/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0002257C */
#include "records.h"

extern unsigned char player_environment;
extern struct collide_probe collide_object_probe;
extern unsigned char world_state_flags;
extern iptr moving_model;
extern iptr floor_object;
extern unsigned char player_on_ground;
extern int collide_candidate_count;
extern struct collide_probe *xn_collide_probe;
extern struct collide_hits *xn_slide_planes;
extern int collide_height;
extern short collide_flags;
extern void collide_for_each_nearby(struct record *, void (*)(iptr));
extern void collide_gather_cb(iptr);
extern int collide_step_object(struct record *, int, struct move_request *);
extern void object_set_position(struct record *, int, int, int, int, int, int);
extern int xn_terrain_height_at(int, int);
extern void xn_vec_normalize_ptr(struct vec3 *);

int collide_move_object(struct record *object, int unused_arg, struct move_request *request, int unused_arg2)
{
    int result;
    int unused40;           /* never used, but it has a stack slot */
    struct vec3 direction;          /* declared here: its place in the list decides the slots */
    int flags;
    int dx;
    int dy;
    int dz;
    int best;
    int i;
    int mindot;
    int dot;
    int unused18;
    iptr saved_cb8;
    iptr saved_c70;

    saved_cb8 = floor_object;
    saved_c70 = moving_model;
    xn_slide_planes = 0;
    collide_candidate_count = 0;
    xn_collide_probe = &collide_object_probe;
    xn_collide_probe->position.x = request->x;
    xn_collide_probe->position.y = request->y;
    xn_collide_probe->position.z = request->z;
    collide_for_each_nearby(object, collide_gather_cb);
    if (player_environment != 1 && collide_candidate_count == 0)
        return 0;
    if (collide_candidate_count == 0 && player_environment == 1) {
        collide_height = xn_terrain_height_at(object->x, object->z);
        object_set_position(object, request->x, collide_height, request->z, request->angle_x, request->yaw, request->angle_z);
        return 0;
    }
    flags = collide_flags;
    player_on_ground = 1;
    result = collide_step_object(object, unused_arg, request);
    world_state_flags &= 223;
    floor_object = saved_cb8;
    moving_model = saved_c70;
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
    flags = collide_flags;
    player_on_ground = 1;
    result = collide_step_object(object, unused_arg, request);
    floor_object = saved_cb8;
    moving_model = saved_c70;
    if (!(result & 10) || !(flags & 4))
        return 0;
    return 1;
}
