/* colstuff.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "bitfield.h"
#include "clib.h"

extern int dungeon_water_level;
extern struct monster_template monster_table[];
extern char str_colstuff_c[];
extern char str_exceeded_maximum_collisions[];
extern unsigned char player_environment;
extern struct record *creature_list[];
extern char scratch_190be4[];
extern signed char scratch_190ce4[];
extern unsigned char world_state_flags;
extern signed char player_motion_flags;
extern char frame_counter[];
extern struct record *player_object;
extern struct arch3d_plane *collide_floor_plane;
extern int vertical_velocity;
extern struct record *location_object;
extern char cheat_flags[];
extern struct record *found_object;
extern int creature_count;
extern struct character *player_character;
extern struct record *collide_hit_object;
extern struct record *moving_model;
extern int ceiling_height;
extern struct record *floor_object;
extern iptr grid_visit_func;
extern iptr collide_side_hit;
extern iptr collide_floor_hit;
extern int nearest_creature_distance;
extern int ai_monster_flags;
extern struct record *nearest_creature;
extern struct arch3d_plane *click_face_texture;
extern signed char player_on_ground;
extern signed char in_dungeon_water;
extern signed char collide_landed;
extern signed char collide_stepped;
extern signed char D_001962A0;
extern int collide_candidate_count;
extern char collide_segment_start[];
extern int collide_segment_start_y;
extern int collide_segment_start_z;
extern char collide_vertical_end[];
extern int collide_vertical_end_y;
extern int collide_vertical_end_z;
extern char collide_target[];
extern int collide_target_y;
extern int collide_target_z;
extern int collide_target_angle_x;
extern int collide_target_yaw;
extern int collide_target_angle_z;
extern struct record *collide_candidates[];
extern struct collide_hits *xn_collide_result;
extern struct collide_probe *xn_collide_probe;
extern iptr xn_slide_planes;
extern char D_00196D54[];
extern int collide_previous_y;
extern int collide_previous_z;
extern int collide_height;
extern char collide_flags[];

extern int object_find_open(struct record *, iptr (*)());
extern int door_start_swing(struct record *, int);
extern int xn_math_approx_dist2d(int, int, int, int);
extern int xn_math_approx_hypot(int, int);
extern iptr xn_collide_segment_model(void *, void *, void *, int);
extern iptr xn_collide_spheres_model(void *, void *, int);
extern iptr xn_collide_segment_flat_stk(void *, void *, void *, unsigned, unsigned, int, int);
extern int xn_terrain_height_at(int, int);
extern unsigned char ground_tile_at(int, int);
extern void collide_for_each_nearby(struct record *, iptr);
extern void fatal_error(char *);
extern void object_set_position(struct record *, int, int, int, int, int, int);
extern void object_move_by(struct record *, int, int, int, int, int, int);
extern void object_foreach(struct record *, void (*)());
extern void object_foreach_open(struct record *, void (*)());
int collide_line_of_sight_cb(struct record *);
int collide_creature_near(struct record *, struct move_request *);
int collide_creature_within(struct record *, int *, int);
void collide_segment_model_cb(struct record *);
void collide_vertical_cb(struct record *);
void func_00022174(struct record *);

int collide_move_missile(struct record *object, int *position, int *angles)
{
    int unused1;
    int unused2;

    unused2 = 0;
    *(signed char *)collide_flags &= 252;
    collide_height = object->y;
    if (((int)player_environment) == 1) {
        if ((collide_height = xn_terrain_height_at(object->x, object->z)) <= object->y) return 2;
    }
    if (collide_creature_within(object, position, 100) != 0) {
        *(signed char *)collide_flags |= 8;
        return 8;
    }
    *(int *)collide_segment_start = object->x;
    collide_segment_start_y = object->y;
    collide_segment_start_z = object->z;
    mc_memcpy(collide_target, position, 12, str_colstuff_c, 61, 4);
    if (angles != 0) mc_memcpy(&collide_target_angle_x, angles, 12, str_colstuff_c, 63, 4);
    collide_for_each_nearby(object, (iptr)collide_segment_model_cb);
    if (((int)(short)(*(short *)collide_flags & 10)) == 0) {
        object_set_position(object, *(int *)collide_target, collide_target_y, collide_target_z, collide_target_angle_x, collide_target_yaw, collide_target_angle_z);
    }
    return (int)(short)*(short *)collide_flags;
}

void collide_segment_model_cb(struct record *object)
{
    char **model;
    struct block *block;
    struct block_model *block_model;
    int unused1;
    int unused2;
    int i;
    int unused3;
    int unused4;

    if ((object->flags & 512) != 0) return;
    switch (object->type) {
    case 43:
        block = &object->data.block;
        block_model = block->models;
        for (i = 0; block->model_count > i; i++, block_model++) {
            if (((int)(short)(*(short *)collide_flags & 4)) != 0 && ((int)(short)(*(short *)collide_flags & 2)) == 0) {
                model = &block_model->model;
                if (*model != 0) {
                    if ((*(iptr *)&xn_collide_result = xn_collide_segment_model(model, collide_segment_start, collide_target, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                        click_face_texture = (struct arch3d_plane *)(*model + xn_collide_result->hits[0].face);
                        *(signed char *)collide_flags |= 2;
                        collide_hit_object = object;
                    }
                }
            }
        }
        return;
    case 56:
        block_model = (struct block_model *)RECORD_DATA(object);
        for (i = 0; object->model_count > i; i++, block_model++) {
            if (((int)(short)(*(short *)collide_flags & 4)) != 0 && ((int)(short)(*(short *)collide_flags & 2)) == 0) {
                model = &block_model->model;
                if (*model != 0) {
                    if ((*(iptr *)&xn_collide_result = xn_collide_segment_model(model, collide_segment_start, collide_target, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                        click_face_texture = (struct arch3d_plane *)(*model + xn_collide_result->hits[0].face);
                        *(signed char *)collide_flags |= 2;
                        collide_hit_object = object;
                    }
                }
            }
        }
        return;
    case 6:
    case 32:
        model = &object->data.instance.model;
        if (*model == 0) return;
        if (((int)(short)(*(short *)collide_flags & 4)) == 0 || ((int)(short)(*(short *)collide_flags & 2)) != 0) {
            return;
        }
        if ((*(iptr *)&xn_collide_result = xn_collide_segment_model(model, collide_segment_start, collide_target, 0)) == 0 || (iptr)xn_collide_result == (-1)) {
            return;
        }
        click_face_texture = (struct arch3d_plane *)(*model + xn_collide_result->hits[0].face);
        *(signed char *)collide_flags |= 2;
        collide_hit_object = object;
    default:;
    }
}

void collide_vertical_cb(struct record *object)
{
    char **model;
    struct block *block;
    struct block_model *block_model;
    int i;
    int j;
    int unused;

    if ((object->flags & 512) != 0 || ((struct bf8_5_1 *)&cheat_flags)->f != 0) return;
    switch (object->type) {
    case 43:
        block = &object->data.block;
        block_model = block->models;
        for (i = 0; block->model_count > i; i++, block_model++) {
            model = &block_model->model;
            if (*model != 0) {
                if ((*(iptr *)&xn_collide_result = xn_collide_segment_model(model, collide_segment_start, collide_vertical_end, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                    for (j = 0; j < xn_collide_result->count; j++) {
                        if ((*(int *)scratch_190be4 != 0 && xn_collide_result->hits[j].y > collide_height) || xn_collide_result->hits[j].y < collide_height) {
                            collide_height = xn_collide_result->hits[j].y;
                            collide_floor_plane = (struct arch3d_plane *)(*model + xn_collide_result->hits[j].face);
                            collide_floor_hit = (iptr)&xn_collide_result->hits[j];
                            floor_object = object;
                        }
                    }
                    *(signed char *)collide_flags |= 1;
                }
            }
        }
        return;
    case 56:
        block_model = (struct block_model *)RECORD_DATA(object);
        for (i = 0; object->model_count > i; i++, block_model++) {
            model = &block_model->model;
            if (*model != 0) {
                if ((*(iptr *)&xn_collide_result = xn_collide_segment_model(model, collide_segment_start, collide_target, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                    for (j = 0; j < xn_collide_result->count; j++) {
                        if ((*(int *)scratch_190be4 != 0 && xn_collide_result->hits[j].y > collide_height) || xn_collide_result->hits[j].y < collide_height) {
                            collide_height = xn_collide_result->hits[j].y;
                            collide_floor_plane = (struct arch3d_plane *)(*model + xn_collide_result->hits[j].face);
                            collide_floor_hit = (iptr)&xn_collide_result->hits[j];
                            floor_object = object;
                        }
                    }
                    *(signed char *)collide_flags |= 1;
                }
            }
        }
        return;
    case 6:
    case 32:
        model = &object->data.instance.model;
        if (*model == 0) return;
        if ((*(iptr *)&xn_collide_result = xn_collide_segment_model(model, collide_segment_start, collide_vertical_end, 0)) == 0 || (iptr)xn_collide_result == (-1)) {
            return;
        }
        for (j = 0; j < xn_collide_result->count; j++) {
            if ((*(int *)scratch_190be4 != 0 && xn_collide_result->hits[j].y > collide_height) || xn_collide_result->hits[j].y < collide_height) {
                collide_height = xn_collide_result->hits[j].y;
                collide_floor_plane = (struct arch3d_plane *)(*model + xn_collide_result->hits[j].face);
                collide_floor_hit = (iptr)&xn_collide_result->hits[j];
                floor_object = object;
            }
        }
        *(signed char *)collide_flags |= 1;
    default:;
    }
}

void func_00022174(struct record *object)
{
    char **model;
    struct block *block;
    struct block_model *block_model;
    int i;
    int unused1;
    struct character *character;
    int unused2;
    int unused3;

    if ((object->flags & 512) != 0 || ((struct bf8_5_1 *)&cheat_flags)->f != 0) return;
    switch (object->type) {
    case 43:
        block = &object->data.block;
        block_model = block->models;
        for (i = 0; block->model_count > i; i++, block_model++) {
            if (((int)(short)(*(short *)collide_flags & 4)) != 0 && ((int)(short)(*(short *)collide_flags & 2)) == 0) {
                model = &block_model->model;
                if (*model != 0) {
                    if ((*(iptr *)&xn_collide_result = xn_collide_spheres_model(model, xn_collide_probe, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                        xn_slide_planes = (iptr)xn_collide_result;
                        click_face_texture = (struct arch3d_plane *)(*model + xn_collide_result->hits[0].face);
                        collide_side_hit = (iptr)xn_collide_result->hits;
                        *(signed char *)collide_flags |= 2;
                        collide_hit_object = object;
                    }
                }
            }
        }
        return;
    case 56:
        block_model = (struct block_model *)RECORD_DATA(object);
        for (i = 0; object->model_count > i; i++, block_model++) {
            if (((int)(short)(*(short *)collide_flags & 4)) != 0 && ((int)(short)(*(short *)collide_flags & 2)) == 0) {
                model = &block_model->model;
                if (*model != 0) {
                    if ((*(iptr *)&xn_collide_result = xn_collide_spheres_model(model, xn_collide_probe, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                        xn_slide_planes = (iptr)xn_collide_result;
                        click_face_texture = (struct arch3d_plane *)(*model + xn_collide_result->hits[0].face);
                        collide_side_hit = (iptr)xn_collide_result->hits;
                        *(signed char *)collide_flags |= 2;
                        collide_hit_object = object;
                    }
                }
            }
        }
        return;
    case 32:
        if ((object->flags & 256) != 0) return;
        if (found_object->type != 18) goto L22468;
        character = &found_object->data.character;
        if ((character->mobile_id & 128) == 0) {
            if (((int)(unsigned short)(monster_table[character->race].flags & 4)) == 0) goto L22468;
        }
        if (object->lock_level != 0) if ((object->flags & 64) == 0) goto L22468;
        if (door_start_swing(object, 0) == 0) goto L22468;
        object->flags |= 0x100;
        return;
    case 6:
L22468:;
        model = &object->data.instance.model;
        if (*model != 0) {
            if (object->move_frame == *(int *)frame_counter || (((int)(short)(*(short *)collide_flags & 4)) != 0 && ((int)(short)(*(short *)collide_flags & 2)) == 0)) {
                if ((*(iptr *)&xn_collide_result = xn_collide_spheres_model(model, xn_collide_probe, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                    xn_slide_planes = (iptr)xn_collide_result;
                    click_face_texture = (struct arch3d_plane *)(*model + xn_collide_result->hits[0].face);
                    collide_side_hit = (iptr)xn_collide_result->hits;
                    *(signed char *)collide_flags |= 2;
                    moving_model = object;
                    collide_hit_object = object;
                }
            }
        }
        return;
    case 18:
        if (found_object == object) return;
        if ((*(iptr *)&xn_collide_result = xn_collide_segment_flat_stk(&object->x, collide_segment_start, collide_target, object->image, 4, 0, 0)) == 0 || (iptr)xn_collide_result == (-1)) {
            return;
        }
        collide_hit_object = object;
        *(signed char *)collide_flags |= 8;
    default:;
    }
}

int collide_step_player(struct record *object, int unused_arg, struct move_request *request)
{
    int unused2;
    int landed;
    int unused3;
    int i;

    landed = 0;
    collide_stepped = 1;
    ceiling_height = object->y - 120;
    *(int *)scratch_190be4 = 0;
    *(int *)((char *)(*(iptr *)&xn_collide_probe = (iptr)request->probe)) = request->x;
    xn_collide_probe->position.y = request->y;
    xn_collide_probe->position.z = request->z;
    *(signed char *)collide_flags &= 228;
    found_object = object;
    D_001962A0 = 0;
    if (((int)player_environment) == 1) {
        collide_height = xn_terrain_height_at(object->x, object->z);
        if (object->y >= collide_height && (signed char)ground_tile_at(request->x, request->z) == 0) {
            D_001962A0 = 1;
        }
    } else {
        collide_height = object->y + 1000;
    }
    mc_memcpy(collide_target, request, 12, str_colstuff_c, 474, 4);
    if (((int)(short)(*(short *)collide_flags & 4)) != 0 && memcmp(request, &object->x, 12) == 0) {
        *(signed char *)collide_flags &= 251;
    }
    if (vertical_velocity > 5120) {
        *(int *)collide_segment_start = *(int *)D_00196D54;
        collide_segment_start_y = collide_previous_y - 20;
        collide_segment_start_z = collide_previous_z;
        *(int *)collide_vertical_end = request->x;
        collide_vertical_end_y = request->y + 20;
        collide_vertical_end_z = request->z;
        for (i = 0; i < collide_candidate_count; i++) {
            collide_vertical_cb(collide_candidates[i]);
        }
    }
    if (vertical_velocity < 0 || ((object == player_object && ((player_character->conditions & 0x8) != 0 || in_dungeon_water != 0)) || ((struct bf8_5_1 *)&player_motion_flags)->f != 0)) {
        *(int *)scratch_190be4 = 1;
        *(signed char *)collide_flags &= 254;
        *(int *)collide_segment_start = request->x;
        collide_segment_start_y = request->y - 60;
        collide_segment_start_z = request->z;
        *(int *)collide_vertical_end = request->x;
        collide_vertical_end_y = request->y - 120;
        collide_vertical_end_z = request->z;
        for (i = 0; i < collide_candidate_count; i++) {
            collide_vertical_cb(collide_candidates[i]);
        }
        *(int *)scratch_190be4 = 0;
        if (((int)(short)(*(short *)collide_flags & 1)) != 0) {
            ceiling_height = collide_height;
        } else {
            ceiling_height = object->y - 120;
        }
        *(signed char *)collide_flags &= 254;
        if (((int)player_environment) == 1) {
            collide_height = xn_terrain_height_at(object->x, object->z);
        } else {
            collide_height = object->y + 1000;
        }
    }
    if (((int)(short)(*(short *)collide_flags & 1)) == 0) {
        *(int *)collide_segment_start = request->x;
        collide_segment_start_y = request->y - 25;
        collide_segment_start_z = request->z;
        *(int *)collide_vertical_end = request->x;
        if (((struct bf8_3_1 *)&player_motion_flags)->f != 0) {
            collide_vertical_end_y = request->y + 140;
        } else {
            collide_vertical_end_y = request->y + 40;
        }
        collide_vertical_end_z = request->z;
        for (i = 0; i < collide_candidate_count; i++) {
            collide_vertical_cb(collide_candidates[i]);
        }
    }
    if (collide_height < request->y) {
        collide_segment_start_y = (collide_target_y = collide_height);
        xn_collide_probe->position.y = collide_segment_start_y;
    }
    for (i = 0; i < collide_candidate_count; i++) {
        func_00022174(collide_candidates[i]);
    }
    if (collide_creature_near(object, request) != 0) *(signed char *)collide_flags |= 8;
    if (((int)(short)(*(short *)collide_flags & 10)) != 0 && ((int)(short)(*(short *)collide_flags & 4)) != 0) {
        if (object == player_object && ((struct bf8_5_1 *)&player_motion_flags)->f != 0) {
            if (request->y < object->y && (request->y - 90) > ceiling_height) {
                object_move_by(object, 0, -(object->y - request->y), 0, 0, object->yaw - request->yaw, 0);
            } else if (request->y > object->y && collide_height > request->y) {
                object_move_by(object, 0, -(object->y - request->y), 0, 0, object->yaw - request->yaw, 0);
            }
        }
        if ((collide_height < request->y && (((int)(short)(*(short *)collide_flags & 1)) != 0 || ((int)player_environment) == 1)) || ((struct bf8_5_1 *)&world_state_flags)->f != 0) {
            world_state_flags &= 223;
            object_move_by(object, 0, collide_height - object->y, 0, 0, 0, 0);
            landed = 1;
        } else if (collide_height != object->y) {
            if (((struct bf8_5_1 *)&player_motion_flags)->f == 0 && collide_landed == 0 && (player_character->conditions & 0x8) == 0 && in_dungeon_water == 0 && (collide_height - object->y) < 30) {
                object_move_by(object, 0, collide_height - object->y, 0, 0, 0, 0);
            } else {
                *(signed char *)collide_flags |= 16;
                if (((int)(short)(*(short *)collide_flags & 1)) == 0) {
                    object_move_by(object, 0, request->y - object->y, 0, 0, 0, 0);
                }
                player_on_ground = 0;
            }
        } else if (((int)(short)(*(short *)collide_flags & 16)) != 0) {
            object_set_position(object, object->x, object->y, object->z, request->angle_x, request->yaw, request->angle_z);
        }
        return (int)(short)*(short *)collide_flags;
    }
    if ((collide_height < request->y && (((int)(short)(*(short *)collide_flags & 1)) != 0 || ((int)player_environment) == 1)) || ((struct bf8_5_1 *)&world_state_flags)->f != 0) {
        world_state_flags &= 223;
        object_move_by(object, 0, collide_height - object->y, 0, 0, 0, 0);
        landed = 1;
    } else if (collide_height >= object->y && ((int)(short)(*(short *)collide_flags & 8)) == 0) {
        if (((struct bf8_5_1 *)&player_motion_flags)->f == 0 && collide_landed == 0 && in_dungeon_water == 0 && (player_character->conditions & 0x8) == 0 && (collide_height - object->y) < 30) {
            object_move_by(object, 0, collide_height - object->y, 0, 0, 0, 0);
        } else {
            if ((request->y - 100) >= ceiling_height) {
                object_move_by(object, 0, request->y - object->y, 0, 0, 0, 0);
            } else if ((request->y - 100) < ceiling_height && request->y > object->y) {
                object_move_by(object, 0, request->y - object->y, 0, 0, 0, 0);
            }
            if (collide_height > object->y) {
                *(signed char *)collide_flags |= 16;
                player_on_ground = 0;
            }
        }
    }
    if (((int)(short)(*(short *)collide_flags & 10)) == 0 && ((int)(short)(*(short *)collide_flags & 4)) != 0) {
        if (((struct bf8_5_1 *)&player_motion_flags)->f == 0) {
            object_set_position(object, *(int *)collide_target, object->y, collide_target_z, request->angle_x, request->yaw, request->angle_z);
        }
    } else if (((struct bf8_5_1 *)&player_motion_flags)->f == 0) {
        object_set_position(object, object->x, object->y, object->z, request->angle_x, request->yaw, request->angle_z);
    }
    *(signed char *)collide_flags &= 251;
    if (object == player_object) {
        *(int *)D_00196D54 = object->x;
        collide_previous_y = object->y;
        collide_previous_z = object->z;
    }
    return (int)(short)*(short *)collide_flags;
}

void collide_gather_cb(struct record *object)
{
    char **model;
    struct block *block;
    struct block_model *block_model;
    int i;
    int unused1;
    int unused2;
    int unused3;

    if ((object->flags & 512) != 0) return;
    switch (object->type) {
    case 43:
        block = &object->data.block;
        block_model = block->models;
        for (i = 0; block->model_count > i; i++, block_model++) {
            model = &block_model->model;
            if (*model != 0) {
                if ((*(iptr *)&xn_collide_result = xn_collide_spheres_model(model, xn_collide_probe, 2)) == 0 && (iptr)xn_collide_result != (-1)) {
                    collide_candidates[collide_candidate_count++] = object;
                    if (collide_candidate_count > 128) fatal_error(str_exceeded_maximum_collisions);
                }
            }
        }
        return;
    case 56:
        block_model = (struct block_model *)RECORD_DATA(object);
        for (i = 0; object->model_count > i; i++, block_model++) {
            model = &block_model->model;
            if (*model != 0) {
                if ((*(iptr *)&xn_collide_result = xn_collide_spheres_model(model, xn_collide_probe, 2)) == 0 && (iptr)xn_collide_result != (-1)) {
                    collide_candidates[collide_candidate_count++] = object;
                    if (collide_candidate_count > 128) fatal_error(str_exceeded_maximum_collisions);
                }
            }
        }
        return;
    case 32:
        if ((object->flags & 256) != 0) return;
    case 6:
        model = &object->data.instance.model;
        if (*model == 0) return;
        if ((*(iptr *)&xn_collide_result = xn_collide_spheres_model(model, xn_collide_probe, 2)) != 0 || (iptr)xn_collide_result == (-1)) {
            return;
        }
        collide_candidates[collide_candidate_count++] = object;
        if (collide_candidate_count <= 128) return;
        fatal_error(str_exceeded_maximum_collisions);
    default:;
    }
}

struct record *collide_find_floor_object(struct record *object)
{
    floor_object = 0;
    collide_for_each_nearby(object, (iptr)collide_vertical_cb);
    return floor_object;
}

int collide_line_of_sight_cb(struct record *object)
{
    char **model;
    struct block *block;
    struct block_model *block_model;
    int i;
    int unused1;
    short unused2;

    if ((object->flags & 512) != 0) return 0;
    switch (object->type) {
    case 43:
        block = &object->data.block;
        block_model = block->models;
        for (i = 0; block->model_count > i; i++, block_model++) {
            model = &block_model->model;
            if (*model != 0) {
                if ((*(iptr *)&xn_collide_result = xn_collide_segment_model(model, collide_segment_start, collide_vertical_end, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                    scratch_190ce4[0] = 1;
                    return 1;
                }
            }
        }
        break;
    case 56:
        block_model = (struct block_model *)RECORD_DATA(object);
        for (i = 0; object->model_count > i; i++, block_model++) {
            model = &block_model->model;
            if (*model != 0) {
                if ((*(iptr *)&xn_collide_result = xn_collide_segment_model(model, collide_segment_start, collide_vertical_end, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                    scratch_190ce4[0] = 1;
                    return 1;
                }
            }
        }
        break;
    case 32:
        if ((object->flags & 256) != 0) break;
    case 6:
        model = &object->data.instance.model;
        if (*model != 0) {
            if ((*(iptr *)&xn_collide_result = xn_collide_segment_model(model, collide_segment_start, collide_vertical_end, 0)) != 0 && (iptr)xn_collide_result != (-1)) {
                scratch_190ce4[0] = 1;
                return 1;
            }
        }
    }
    return 0;
}

int collide_line_of_sight(struct record *from, struct record *to)
{
    scratch_190ce4[0] = 0;
    mc_memcpy(collide_segment_start, &from->x, 12, str_colstuff_c, 923, 4);
    mc_memcpy(collide_vertical_end, &to->x, 12, str_colstuff_c, 924, 4);
    collide_segment_start_y -= 40;
    collide_vertical_end_y -= 40;
    grid_visit_func = (iptr)object_find_open;
    collide_for_each_nearby(from, (iptr)collide_line_of_sight_cb);
    grid_visit_func = (iptr)object_foreach_open;
    return (int)(signed char)(scratch_190ce4[0] ^ 1);
}

void creatures_find_nearest(void)
{
    int best;
    int i;
    int distance;

    best = 10000;
    nearest_creature = 0;
    for (i = 0; i < creature_count; i++) {
        distance = xn_math_approx_hypot(player_object->y - creature_list[i]->y, xn_math_approx_dist2d(player_object->x, player_object->z, creature_list[i]->x, creature_list[i]->z));
        if (distance < best) {
            nearest_creature_distance = distance;
            nearest_creature = (struct record *)(creature_list[i]);
            best = distance;
        }
    }
}

int collide_creature_near(struct record *object, struct move_request *request)
{
    int i;
    int dy;

    creature_list[creature_count] = player_object;
    for (i = 0; i <= creature_count; i++) {
        if (creature_list[i] == object) continue;
        dy = creature_list[i]->y - request->y;
        if (dy < 0 || dy > 100) continue;
        if (xn_math_approx_dist2d(creature_list[i]->x, creature_list[i]->z, request->x, request->z) < 50) {
            collide_hit_object = (struct record *)(creature_list[i]);
            return 1;
        }
    }
    return 0;
}

int collide_creature_within(struct record *object, int *position, int radius)
{
    int i;
    int dy;

    creature_list[creature_count] = player_object;
    for (i = 0; i <= creature_count; i++) {
        if (creature_list[i] == object) continue;
        dy = creature_list[i]->y - position[1];
        if (dy < 0 || dy > 100) continue;
        if (xn_math_approx_dist2d(creature_list[i]->x, creature_list[i]->z, position[0], position[2]) < radius) {
            collide_hit_object = (struct record *)(creature_list[i]);
            return 1;
        }
    }
    return 0;
}

int func_00023FA5(struct record *object, int unused_arg, struct move_request *request)
{
    int unused2;
    int landed;
    int unused3;
    int i;
    struct character *character;

    landed = 0;
    collide_stepped = 0;
    ceiling_height = object->y - 120;
    *(int *)scratch_190be4 = 0;
    character = &object->data.character;
    *(int *)((char *)(*(iptr *)&xn_collide_probe = (iptr)request->probe)) = request->x;
    xn_collide_probe->position.y = request->y;
    xn_collide_probe->position.z = request->z;
    *(signed char *)collide_flags &= 228;
    found_object = object;
    if (((int)player_environment) == 1) {
        collide_height = xn_terrain_height_at(object->x, object->z);
    } else {
        collide_height = object->y + 1000;
    }
    mc_memcpy(collide_target, request, 12, str_colstuff_c, 1021, 4);
    if (((int)(short)(*(short *)collide_flags & 4)) != 0 && memcmp(request, &object->x, 12) == 0) {
        *(signed char *)collide_flags &= 251;
    }
    if (character->fall_velocity > 5120) {
        *(int *)collide_segment_start = *(int *)D_00196D54;
        collide_segment_start_y = collide_previous_y - 20;
        collide_segment_start_z = collide_previous_z;
        *(int *)collide_vertical_end = request->x;
        collide_vertical_end_y = request->y;
        collide_vertical_end_z = request->z;
        for (i = 0; i < collide_candidate_count; i++) {
            collide_vertical_cb(collide_candidates[i]);
        }
    }
    if (character->fall_velocity < 0 || ((struct bf8_0_1 *)&ai_monster_flags)->f != 0) {
        *(int *)scratch_190be4 = 1;
        *(int *)collide_segment_start = object->x;
        collide_segment_start_y = object->y - 60;
        collide_segment_start_z = object->z;
        *(int *)collide_vertical_end = object->x;
        collide_vertical_end_y = object->y - 120;
        collide_vertical_end_z = object->z;
        for (i = 0; i < collide_candidate_count; i++) {
            collide_vertical_cb(collide_candidates[i]);
        }
        *(int *)scratch_190be4 = 0;
        if (((int)(short)(*(short *)collide_flags & 1)) != 0) {
            ceiling_height = collide_height;
        } else {
            ceiling_height = object->y - 120;
        }
        if (dungeon_water_level != 10000 && dungeon_water_level > ceiling_height && request->y > dungeon_water_level) {
            ceiling_height = dungeon_water_level;
        }
        *(signed char *)collide_flags &= 254;
        collide_height = object->y + 1000;
    }
    if (((int)(short)(*(short *)collide_flags & 1)) == 0) {
        *(int *)collide_segment_start = object->x;
        collide_segment_start_y = object->y - 25;
        collide_segment_start_z = object->z;
        *(int *)collide_vertical_end = object->x;
        if (((struct bf8_3_1 *)&player_motion_flags)->f != 0) {
            collide_vertical_end_y = object->y + 140;
        } else {
            collide_vertical_end_y = object->y + 40;
        }
        collide_vertical_end_z = object->z;
        for (i = 0; i < collide_candidate_count; i++) {
            collide_vertical_cb(collide_candidates[i]);
        }
    }
    if (collide_height < request->y) {
        collide_segment_start_y = (collide_target_y = collide_height);
        xn_collide_probe->position.y = collide_segment_start_y;
    }
    for (i = 0; i < collide_candidate_count; i++) {
        func_00022174(collide_candidates[i]);
    }
    if (((int)player_environment) != 2 && collide_creature_near(object, request) != 0) {
        *(signed char *)collide_flags |= 8;
    }
    if (((int)(unsigned short)(request->flags & 1)) == 0 && ((struct bf8_7_1 *)&world_state_flags)->f != 0 && ((struct bf8_0_1 *)&ai_monster_flags)->f == 0) {
        world_state_flags &= 127;
        if ((collide_height - request->y) > 60) {
            *(signed char *)collide_flags |= 2;
            return (int)(short)*(short *)collide_flags;
        }
    }
    if (((int)(short)(*(short *)collide_flags & 10)) != 0 && ((int)(short)(*(short *)collide_flags & 4)) != 0) {
        return (int)(short)*(short *)collide_flags;
    }
    if ((collide_height - request->y) < 30 && ((struct bf8_0_1 *)&ai_monster_flags)->f == 0) {
        object_move_by(object, 0, collide_height - object->y, 0, 0, 0, 0);
    }
    if ((collide_height < request->y && (collide_height - 90) > ceiling_height && (((int)(short)(*(short *)collide_flags & 1)) != 0 || ((int)player_environment) == 1)) || ((struct bf8_5_1 *)&world_state_flags)->f != 0) {
        world_state_flags &= 223;
        object_move_by(object, 0, collide_height - object->y, 0, 0, 0, 0);
        landed = 1;
    } else if (collide_height != object->y) {
        *(signed char *)collide_flags |= 16;
        player_on_ground = 0;
    }
    if (((int)(short)(*(short *)collide_flags & 10)) == 0 && ((int)(short)(*(short *)collide_flags & 4)) != 0) {
        object_set_position(object, *(int *)collide_target, object->y, collide_target_z, request->angle_x, request->yaw, request->angle_z);
    } else {
        object_set_position(object, object->x, object->y, object->z, request->angle_x, request->yaw, request->angle_z);
    }
    *(signed char *)collide_flags &= 251;
    return (int)(short)*(short *)collide_flags;
}

int collide_floor_height(struct record *object)
{
    mc_memcpy(collide_segment_start, &object->x, 12, str_colstuff_c, 1143, 4);
    mc_memcpy(collide_vertical_end, &object->x, 12, str_colstuff_c, 1144, 4);
    collide_segment_start_y -= 20;
    collide_vertical_end_y += 40;
    collide_height = 100000;
    object_foreach(location_object->children, collide_vertical_cb);
    return collide_height;
}
