/* fs2df.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"

extern char str_fs2df_c[];
extern char str_info01i0_img[];
extern char str_mask01i0_img[];
extern char str_mask05i0_img[];
extern signed char ui_dirty_flags;
extern iptr magic_window_image;
extern iptr window_image;
extern unsigned char game_mode_under_popup;
extern signed char screen_active;
extern signed char game_mode;
extern struct rdb_object_id rdb_object_ids[];
extern struct link *rdb_chain_link;
extern char *rdb_data;
extern struct link *rdb_current_link;
extern struct rdb_flat *rdb_flat_resource;
extern struct rdb_light *rdb_light_resource;
extern struct rdb_action *rdb_action_resource;
extern struct rdb_file *rdb_loaded_file;
extern struct rdb_model *rdb_model_resource;
extern int rdb_object_id_count;
extern short rdb_link_object_id;
extern iptr spellmaker_settings_image;
extern struct link links[1024];
extern int link_count;

extern int spellmaker_new(void);
extern iptr disk_read_file(char *, iptr);
short rdb_object_id_by_offset(int);
void rdb_model_find_action(struct rdb_model *);
void rdb_build_action_chain(int);
void action_record_add(struct rdb_model *, struct rdb_action *, struct rdb_flat *, unsigned char);
void action_record_add_chained(struct rdb_model *, struct rdb_action *, struct rdb_flat *, unsigned char);
void action_axis_to_translation(struct link *);

void rdb_model_find_action(struct rdb_model *model)
{
    struct rdb_unknown_entry *entry;
    int key;

    key = 0;
    entry = (struct rdb_unknown_entry *)(rdb_data + rdb_loaded_file->object_header.unknown_offset);
    while (entry->key != key) {
        entry = (struct rdb_unknown_entry *)(rdb_data + entry->next);
    }
    model->action_offset = entry->action_offset;
    model->trigger_flag_starting_lock = entry->trigger_flag_starting_lock;
    model->sound_index = entry->sound_index;
}

void rdb_link_actions(struct record *quarter, struct rdb_object *rdb_object, int block_index)
{
    int unused1;
    int unused2;
    int offset;

    do {
        offset = (int)((char *)rdb_object - rdb_data);
        switch (rdb_object->type) {
        case 1:
            if ((rdb_model_resource = (struct rdb_model *)(rdb_data + rdb_object->resource_offset))->action_offset < 0) {
                rdb_model_find_action(rdb_model_resource);
            }
            if (rdb_model_resource->trigger_flag_starting_lock != 0) {
                rdb_action_resource = (struct rdb_action *)(rdb_data + rdb_model_resource->action_offset);
                rdb_link_object_id = rdb_object_id_by_offset(offset);
                rdb_build_action_chain(rdb_object->type);
            }
            break;
        case 2:
            rdb_light_resource = (struct rdb_light *)(rdb_data + rdb_object->resource_offset);
            break;
        case 3:
            if (((rdb_flat_resource = (struct rdb_flat *)(rdb_data + rdb_object->resource_offset))->flags != 0 && rdb_flat_resource->image != 25482) || (rdb_flat_resource->image == 25490 && rdb_flat_resource->magnitude == 70 && rdb_flat_resource->next_object_offset == 16747 && rdb_flat_resource->action == 5)) {
                if (rdb_flat_resource->image != 25488) {
                    rdb_link_object_id = rdb_object_id_by_offset(offset);
                    rdb_build_action_chain(rdb_object->type);
                }
            }
        }
        rdb_object = (struct rdb_object *)(rdb_data + rdb_object->next);
    } while (((char *)rdb_object - rdb_data) > 0);
}

short rdb_object_id_by_offset(int offset)
{
    int i;

    for (i = 0; i < rdb_object_id_count; i++) {
        if (rdb_object_ids[i].offset == offset) return rdb_object_ids[i].id;
    }
    return 0;
}

void rdb_build_action_chain(int resource_type)
{
    struct rdb_object *rdb_object;
    int offset;

    switch ((unsigned)resource_type) {
    case 1:
        action_record_add(rdb_model_resource, rdb_action_resource, 0, 0);
        offset = rdb_action_resource->next_object_offset;
        break;
    case 2:
        action_record_add(0, 0, 0, rdb_light_resource->action);
        offset = rdb_light_resource->next_object_offset;
        break;
    case 3:
        action_record_add(0, 0, rdb_flat_resource, rdb_flat_resource->action);
        offset = rdb_flat_resource->next_object_offset;
    }
    while (offset > 0) {
        rdb_link_object_id = rdb_object_id_by_offset(offset);
        rdb_object = (struct rdb_object *)(rdb_data + offset);
        switch (rdb_object->type) {
        case 1:
            if ((rdb_model_resource = (struct rdb_model *)(rdb_data + rdb_object->resource_offset))->action_offset < 0) {
                rdb_model_find_action(rdb_model_resource);
            }
            rdb_action_resource = (struct rdb_action *)(rdb_data + rdb_model_resource->action_offset);
            action_record_add_chained(rdb_model_resource, rdb_action_resource, 0, 0);
            offset = rdb_action_resource->next_object_offset;
            break;
        case 2:
            action_record_add_chained(0, 0, 0, (rdb_light_resource = (struct rdb_light *)(rdb_data + rdb_object->resource_offset))->action);
            offset = rdb_light_resource->next_object_offset;
            break;
        case 3:
            action_record_add_chained(0, 0, rdb_flat_resource, (rdb_flat_resource = (struct rdb_flat *)(rdb_data + rdb_object->resource_offset))->action);
            offset = rdb_flat_resource->next_object_offset;
        }
    }
}

void action_record_add(struct rdb_model *model, struct rdb_action *model_action, struct rdb_flat *flat, unsigned char action)
{
    rdb_current_link = (rdb_chain_link = &links[link_count++]);
    mc_memset(rdb_current_link, 0, REC_SIZEOF(struct link), str_fs2df_c, 447, 4);
    rdb_current_link->object_id = rdb_link_object_id;
    if (model_action != 0) {
        rdb_current_link->trigger = model->trigger_flag_starting_lock;
        rdb_current_link->param = model->sound_index;
        rdb_current_link->axis = model_action->axis;
        rdb_current_link->duration = model_action->duration;
        rdb_current_link->magnitude = model_action->magnitude;
        rdb_current_link->action = model_action->action;
    } else if (flat != 0) {
        rdb_current_link->trigger = flat->flags;
        rdb_current_link->param = flat->sound_index;
        rdb_current_link->axis = flat->magnitude;
        rdb_current_link->action = action;
    } else {
        rdb_current_link->action = action;
    }
    if (rdb_current_link->action > 1 && rdb_current_link->action < 8) action_axis_to_translation(rdb_current_link);
    rdb_current_link->chain_count = 0;
}

void action_record_add_chained(struct rdb_model *model, struct rdb_action *model_action, struct rdb_flat *flat, unsigned char action)
{
    rdb_chain_link->chain_count++;
    rdb_current_link = &links[link_count++];
    mc_memset(rdb_current_link, 0, REC_SIZEOF(struct link), str_fs2df_c, 490, 4);
    rdb_current_link->object_id = rdb_link_object_id;
    if (model_action != 0) {
        rdb_current_link->trigger = model->trigger_flag_starting_lock;
        rdb_current_link->param = model->sound_index;
        rdb_current_link->axis = model_action->axis;
        rdb_current_link->duration = model_action->duration;
        rdb_current_link->magnitude = model_action->magnitude;
        rdb_current_link->action = model_action->action;
    } else if (flat != 0) {
        rdb_current_link->trigger = flat->flags;
        rdb_current_link->param = flat->sound_index;
        rdb_current_link->axis = flat->magnitude;
        rdb_current_link->action = action;
    } else {
        rdb_current_link->action = action;
    }
    if (rdb_current_link->action <= 1 || rdb_current_link->action >= 8) return;
    action_axis_to_translation(rdb_current_link);
}

void action_axis_to_translation(struct link *link)
{
    link->magnitude = link->axis << 3;
    link->axis = ((link->action - 2) ^ 1) + 1;
    link->duration = 50;
    link->action = 1;
}

int spellmaker_open(int opening)
{
    if (((int)game_mode_under_popup) == 2) return 1;
    if (opening != 0) {
        game_mode = 2;
        window_image = disk_read_file(str_info01i0_img, 0);
        magic_window_image = disk_read_file(str_mask01i0_img, 0);
        spellmaker_settings_image = disk_read_file(str_mask05i0_img, 0);
        screen_active = 1;
        ui_dirty_flags |= 1;
        spellmaker_new();
    }
    return ((((int)(unsigned char)game_mode) == 2) ? 1 : 0);
}
