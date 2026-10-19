/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0008DD7E */
#include "records.h"

extern int object_move_new;
extern int object_move_new_y;
extern int object_move_new_z;
extern short object_move_new_angle_x;
extern short object_move_new_yaw;
extern short object_move_new_angle_z;
extern short object_move_old_angles;
extern short object_move_old_yaw;
extern short object_move_old_angle_z;
extern int object_move_old_z;
extern int object_move_old_x;
extern int object_move_old_y;
extern void rotate_xz(int *, int *, int);

void object_follow_move_cb(struct record *object)
{
    int unused;
    int dx;
    int dz;

    if ((iptr)object->parent == 3 || (iptr)object->parent == 18)
        return;
    if (object->type == 52 || object->parent->type == 52 || object->parent->type == 22)
        return;
    object->angle_x = object_move_new_angle_x + (object->angle_x - object_move_old_angles) & 2047;
    object->yaw = object_move_new_yaw + (object->yaw - object_move_old_yaw) & 2047;
    object->angle_z = object_move_new_angle_z + (object->angle_z - object_move_old_angle_z) & 2047;
    dx = object->x - object_move_old_x;
    dz = object->z - object_move_old_z;
    rotate_xz(&dx, &dz, object->yaw);
    object->x = object_move_new + dx;
    object->y = object_move_new_y + (object->y - object_move_old_y);
    object->z = object_move_new_z + dz;
}
