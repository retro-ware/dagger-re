/* args.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "bitfield.h"
#include "clib.h"
#include "doslow.h"

extern int D_000C5404;
extern int xn_sin_table[];
extern int xn_cos_table[];
extern int cfg_helmet;
extern char str_args_c[];
extern char str_r[];
extern char str_fmt_s_s_10[];
extern char str_path[];
extern char D_00176A9C[];
extern char str_pathcd[];
extern char str_controls[];
extern char str_fadecolor[];
extern char str_mapfile[];
extern char str_faction[];
extern char str_fpu[];
extern char str_startmap[];
extern char str_artifact[];
extern char str_facloop[];
extern char str_custom[];
extern char str_item[];
extern char str_gender[];
extern char str_ps2fix[];
extern char str_user[];
extern char str_region[];
extern char str_debug[];
extern char str_blockstr[];
extern char str_helmet[];
extern char str_maps_2[];
extern char str_marker[];
extern char str_seed[];
extern char str_stereo[];
extern char str_objmemsize[];
extern char str_texturememory[];
extern char str_magicrepair[];
extern char str_cheatmode[];
extern char str_err_path_not_specified[];
extern char str_err_map_file_not_specified[];
extern char str_there_are_configuration_error_s[];
extern char str_memory_checking_is_active_for_this[];
extern char str_memory_checking_is_not_active_for_this[];
extern iptr controls_file;
extern unsigned char player_environment;
extern int cfg_texture_memory;
extern int mem_check_level;
extern char scratch_190be4[];
extern int scratch_190be8;
extern int scratch_190bec;
extern signed char D_001917E3[];
extern char arena2_path[];
extern signed char D_00191833[];
extern char arena2_cd_path[];
extern char cfg_last_path[];
extern signed char player_motion_flags;
extern struct record *player_object;
extern struct record *location_object;
extern char cheat_flags[];
extern struct location *current_location;
extern struct character *player_character;
extern signed char cfg_map_file;
extern char cfg_item_file[];
extern char classmaker_file[];
extern int spell_cast_queue_count;
extern int free_later_count;
extern int cfg_magic_repair;
extern signed char climate_weathers[];
extern signed char D_001961AE;
extern signed char cfg_user;
extern signed char cfg_fade_colour;
extern signed char current_climate;
extern signed char cfg_artifact;
extern signed char cfg_fpu;
extern signed char cfg_ps2fix;
extern signed char cfg_stereo;
extern int rmb_origin_z;
extern int rmb_origin_y;
extern int rmb_origin_x;
extern char rmb_origin_yaw[];
extern struct block *rmb_record_ptr;
extern struct record *automap_selected_object;
extern char cfg_block_str[];
extern char cfg_mapsave_file[];
extern int cfg_seed;
extern int cfg_start_map;
extern short cfg_facloop;
extern short cfg_faction;
extern signed char cfg_show_markers;
extern signed char cfg_debug;
extern signed char cheat_mode;
extern signed char cfg_gender;
extern signed char cfg_flags;
extern signed char cfg_region;
extern signed char D_001A949C;
extern signed char model_cache_flush_count;
extern int object_heap_size;

extern int climate_category(void);
extern int weapon_arrow_update(struct record *);
extern int object_draw_cb(struct record *);
extern struct record *rmb_make_door(struct record *, short, short, int);
extern struct record *rmb_make_flat(struct record *, short, short, int);
extern iptr model_get(int, int, int);
extern struct record *object_create_child(struct record *, struct record *, int);
extern void xn_model_set_angles(int, int, int, short *);
extern void xn_model_compose_angles(short *, int, int, int);
extern void xn_model_set_angles_yaw_offset(short *, int);
extern int xn_math_approx_dist2d(int, int, int, int);
extern int xn_math_fixmul28(int, int);
extern int xn_light_add(int, int, int, int, int, int);
extern int xn_model_submit(void *, int);
extern iptr xn_flat_add(int, int, int, unsigned, int, unsigned, unsigned);
extern void sky_apply_sunlight(void);
extern void weapon_missile_orient(struct record *);
extern void object_free_pending(void);
extern void spell_cast_queued_run(void);
extern void func_0007E815(struct record *, void (*)());
extern void town_grid_visit_near(struct record *, void (*)());
extern void player_light_draw(void);
extern void object_foreach_open(struct record *, void (*)());
void rotate_xz(int *, int *, int);
#pragma aux func_0009DA1C parm routine [];
#pragma aux mc_set_location parm routine [];

void config_read(char *path)
{
    iptr file;
    unsigned char missing;
    {
        char key[32];
        char value[32];

        missing = 0;
        func_0009DA1C(62, str_args_c);
        file = (iptr)fopen(path, str_r);
        if (file != 0) {
            while (fprintf_2((void *)file, str_fmt_s_s_10, (iptr)key, (iptr)value) != (-1)) {
                if (stricmp(key, str_path) == 0) {
                    mc_strncpy(arena2_path, value, 80, str_args_c, 70);
                    if (((int)(unsigned char)D_001917E3[strlen(arena2_path)]) != 92) {
                        func_000A1054(arena2_path, D_00176A9C, str_args_c, 72, 80);
                    }
                    mc_strncpy(cfg_last_path, value, 80, str_args_c, 73);
                    cfg_flags |= 1;
                } else if (stricmp(key, str_pathcd) == 0) {
                    mc_strncpy(arena2_cd_path, value, 80, str_args_c, 78);
                    if (((int)(unsigned char)D_00191833[strlen(arena2_cd_path)]) != 92) {
                        func_000A1054(arena2_cd_path, D_00176A9C, str_args_c, 80, 80);
                    }
                    mc_strncpy(cfg_last_path, value, 80, str_args_c, 81);
                    cfg_flags |= 1;
                } else if (stricmp(key, str_controls) == 0) {
                    mc_strncpy((char *)controls_file, value, 4, str_args_c, 86);
                } else if (stricmp(key, str_fadecolor) == 0) {
                    cfg_fade_colour = atoi(value);
                } else if (stricmp(key, str_mapfile) == 0) {
                    mc_strncpy((char *)&cfg_map_file, value, 12, str_args_c, 94);
                    cfg_flags |= 4;
                } else if (stricmp(key, str_faction) == 0) {
                    cfg_faction = atoi(value);
                } else if (stricmp(key, str_fpu) == 0) {
                    cfg_fpu = atoi(value);
                } else if (stricmp(key, str_startmap) == 0) {
                    cfg_start_map = atoi(value);
                } else if (stricmp(key, str_artifact) == 0) {
                    cfg_artifact = atoi(value);
                } else if (stricmp(key, str_facloop) == 0) {
                    cfg_facloop = atoi(value);
                } else if (stricmp(key, str_custom) == 0) {
                    mc_strncpy(classmaker_file, value, 20, str_args_c, 119);
                } else if (stricmp(key, str_item) == 0) {
                    mc_strncpy(cfg_item_file, value, 20, str_args_c, 123);
                } else if (stricmp(key, str_gender) == 0) {
                    cfg_gender = atoi(value);
                } else if (stricmp(key, str_ps2fix) == 0) {
                    cfg_ps2fix = atoi(value);
                } else if (stricmp(key, str_user) == 0) {
                    cfg_user = atoi(value);
                } else if (stricmp(key, str_region) == 0) {
                    cfg_region = atoi(value);
                } else if (stricmp(key, str_debug) == 0) {
                    cfg_debug = atoi(value);
                } else if (stricmp(key, str_blockstr) == 0) {
                    mc_strncpy(cfg_block_str, value, 80, str_args_c, 147);
                } else if (stricmp(key, str_helmet) == 0) {
                    cfg_helmet = atoi(value);
                } else if (stricmp(key, str_maps_2) == 0) {
                    mc_strncpy(cfg_mapsave_file, value, 80, str_args_c, 155);
                } else if (stricmp(key, str_marker) == 0) {
                    cfg_show_markers = atoi(value);
                } else if (stricmp(key, str_seed) == 0) {
                    cfg_seed = atoi(value);
                } else if (stricmp(key, str_stereo) == 0) {
                    cfg_stereo = atoi(value);
                } else if (stricmp(key, str_objmemsize) == 0) {
                    object_heap_size = NATIVE_HEAP_BYTES(atoi(value) << 10);
                } else if (stricmp(key, str_texturememory) == 0) {
                    cfg_texture_memory = atoi(value);
                } else if (stricmp(key, str_magicrepair) == 0) {
                    cfg_magic_repair = atoi(value);
                } else if (stricmp(key, str_cheatmode) == 0) {
                    cheat_mode = atoi(value);
                }
            }
            func_0009DA1C(196, str_args_c);
            fclose((void *)file);
        }
        if (((int)(unsigned char)(cfg_flags & 1)) == 0) {
            func_0009DA1C(201, str_args_c);
            printf(str_err_path_not_specified);
            missing = 1;
        }
        if (((int)(unsigned char)(cfg_flags & 4)) == 0) {
            func_0009DA1C(207, str_args_c);
            printf(str_err_map_file_not_specified);
            missing = 1;
        } else if (((int)(unsigned char)cfg_map_file) == 100) {
            player_environment = 3;
        }
        if (missing != 0) {
            func_0009DA1C(216, str_args_c);
            printf(str_there_are_configuration_error_s);
            exit(5);
        }
        if (mem_check_level != 0) {
            mc_set_location(221, str_args_c);
            func_000A148C(str_memory_checking_is_active_for_this);
            return;
        }
        mc_set_location(223, str_args_c);
        func_000A148C(str_memory_checking_is_not_active_for_this);
    }
}

void world_draw_objects(void)
{
    int unused;
    int unused2;
    int unused3;
    struct record *next;
    struct record *block;
    struct record *children;
    uslot16 flags;

    D_001A949C = climate_weathers[climate_category()];
    model_cache_flush_count = 0;
    free_later_count = 0;
    spell_cast_queue_count = 0;
    if (D_001961AE != 0) object_draw_cb((struct record *)&D_001961AE);
    if (((int)player_environment) < 3) {
        if (((struct bf8_0_1 *)&player_motion_flags)->f == 0) {
            if ((player_character->conditions & 0x10) != 0) {
                player_light_draw();
            } else {
                xn_light_add(player_object->x, player_object->y - 60, player_object->z, 16, 128, 0);
            }
        }
        if (player_object->parent->type != 1) {
            object_foreach_open(player_object->parent->children, object_draw_cb);
        } else {
            town_grid_visit_near(player_object, object_draw_cb);
        }
        block = location_object->children;
        while (block != 0) {
            next = block->next;
            children = block->children;
            flags = block->flags;
            if (block->type != 38) {
                object_draw_cb(block);
                if (((int)(unsigned short)(*(int *)&flags & 1)) == 0) {
                    object_foreach_open(children, object_draw_cb);
                }
            }
            block = next;
        }
        if (((int)player_environment) == 1) sky_apply_sunlight();
    } else {
        if (((struct bf8_0_1 *)&player_motion_flags)->f == 0) {
            if ((player_character->conditions & 0x10) != 0) {
                player_light_draw();
            } else {
                xn_light_add(player_object->x, player_object->y - 60, player_object->z, 16, 128, 0);
            }
        }
        func_0007E815(player_object, object_draw_cb);
        block = location_object->children;
        while (block != 0) {
            next = block->next;
            children = block->children;
            flags = block->flags;
            if (block->type != 47) {
                object_draw_cb(block);
                if (((int)(unsigned short)(*(int *)&flags & 1)) == 0) {
                    object_foreach_open(children, object_draw_cb);
                }
            }
            block = next;
        }
    }
    spell_cast_queued_run();
    object_free_pending();
}

void rotate_xz(int *px, int *pz, int yaw)
{
    int x;
    int z;

    x = *px;
    z = *pz;
    yaw &= 2047;
    *px = xn_math_fixmul28(x, xn_cos_table[yaw]) - xn_math_fixmul28(z, xn_sin_table[yaw]);
    *pz = xn_math_fixmul28(z, xn_cos_table[yaw]) + xn_math_fixmul28(x, xn_sin_table[yaw]);
}

int automap_draw_object_cb(struct record *object)
{
    struct model_instance *instance;
    struct block *block;
    struct block_flat *flats;
    struct block_model *model;
    int unused;
    int unused2;
    int i;
    int unused3;
    int unused4;
    pslot16 ticks_addr;

    if (model_cache_flush_count != 0) return 1;
    if ((object->flags & 512) != 0) return 0;
    if ((object->flags & 1024) != 0) return 0;
    if (((int)player_environment) != 2) {
        if (object->type == 34 && ((object->image & 31) - 2) == 8) {
        } else {
            if ((object->flags & 128) == 0 && ((struct bf8_4_1 *)&cheat_flags)->f == 0) return 0;
            if (object->type != 6 && object->type != 32) return 0;
            *(iptr *)&ticks_addr = (iptr)DOS_LOW(0x46C);
            if ((iptr)automap_selected_object == (iptr)object && ((struct bf8_3_1 *)(*(char **)&ticks_addr))->f == 0) {
                return 0;
            }
        }
    }
    if (abs(object->y - scratch_190be8) > 700) return 0;
    if (xn_math_approx_dist2d(object->x, object->z, *(int *)scratch_190be4, scratch_190bec) > 2048) return 0;
    object->draw_handle = 0;
    D_000C5404 = 0;
    switch (object->type) {
    case 34:
        if (cfg_show_markers == 0) break;
        if (object->image == 0 || object->image == 65535) break;
        object->draw_handle = xn_flat_add(object->x, object->y, object->z, object->image, -1, 4, 4129024);
        break;
    case 43:
        block = &object->data.block;
        model = block->models;
        flats = block->flats;
        for (i = 0; block->model_count > i; i++, model++) {
            if (abs(model->y - scratch_190be8) > 100) continue;
            model->model = (char *)model_get(model->id, model->variant, (((int)(unsigned char)current_climate) << 2) + ((int)(unsigned char)D_001A949C));
            if (model->model != 0) xn_model_submit(&model->model, 0);
        }
    case 6:
    case 32:
        if (object->image2 == 0) break;
        instance = &object->data.instance;
        instance->model = (char *)model_get(object->image2, object->image, (((int)(unsigned char)current_climate) << 2) + ((int)(unsigned char)D_001A949C));
        if (instance->model != 0) {
            instance->x = object->x;
            instance->y = object->y;
            instance->z = object->z;
            if (object->image2 == 998) {
                if (weapon_arrow_update(object) == 0) break;
                weapon_missile_orient(object);
                instance->missile_angles[0] = object->missile_yaw;
                instance->missile_angles[1] = object->angle_z;
                instance->missile_angles[2] = 0;
            } else if (object->link_flag != 255) {
                xn_model_set_angles_yaw_offset(instance->angles, object->wait_state);
            } else {
                if (object->image2 == 610 && object->image == 32) D_000C5404 = 3;
                xn_model_compose_angles(instance->angles, object->angle_x, object->yaw + object->wait_state, object->angle_z);
            }
            xn_model_submit(instance, 0);
        }
        break;
    case 56:
        model = (struct block_model *)RECORD_DATA(object);
        for (i = 0; object->model_count > i; i++, model++) {
            if (abs(model->y - scratch_190be8) > 100) continue;
            model->model = (char *)model_get(model->id, model->variant, (((int)(unsigned char)current_climate) << 2) + ((int)(unsigned char)D_001A949C));
            if (model->model != 0) xn_model_submit(&model->model, 0);
        }
    }
    return 0;
}

struct record *rmb_make_light(struct record *parent, int image, int radius)
{
    struct record *light;

    light = object_create_child(parent, 0, 0);
    light->type = 7;
    light->image = image;
    light->light_radius = radius;
    light->pad13 = 8000;
    light->id = location_object->id + ((int)(unsigned short)(current_location->object_counter)++);
    return light;
}

void rmb_add_doors(struct record *parent, struct block_door *door)
{
    struct record *object;
    int i;

    if (rmb_record_ptr->door_count == 0) return;
    for (i = 0; rmb_record_ptr->door_count > i; i++, door++) {
        object = rmb_make_door(parent, door->image2, (int)(short)((unsigned short)door->image), 1);
        xn_model_set_angles(0, (door->yaw + *(int *)rmb_origin_yaw) % 2048, 0, (short *)object->data.instance.angles);
        object->lock_level = (unsigned short)door->lock_level;
        rotate_xz(&door->x, &door->z, *(int *)rmb_origin_yaw);
        door->x += rmb_origin_x;
        object->x = door->x;
        door->z += rmb_origin_z;
        object->z = door->z;
        door->y += rmb_origin_y;
        object->y = door->y;
    }
}

void rmb_add_people(struct record *parent, struct block_flat *person)
{
    struct record *object;
    int i;

    if (rmb_record_ptr->people_count == 0) return;
    for (i = 0; rmb_record_ptr->people_count > i; i++, person++) {
        object = rmb_make_flat(parent, (int)(short)person->image, (int)(short)person->faction_id, 0);
        rotate_xz(&person->x, &person->z, *(int *)rmb_origin_yaw);
        person->x += rmb_origin_x;
        object->x = person->x;
        person->z += rmb_origin_z;
        object->z = person->z;
        person->y += rmb_origin_y;
        object->y = person->y;
        if (((int)(unsigned char)(person->flags & 4)) != 0) object->data.person.flags |= 8;
        if (((int)(unsigned char)(person->flags & 8)) != 0) object->data.person.flags |= 32;
        if (((int)(unsigned char)(person->flags & 32)) != 0) object->data.person.flags |= 16;
    }
}
