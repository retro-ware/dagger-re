/* loadsave.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"
#include "portio.h"

extern signed char mouse_buttons;
extern signed char mouse_double_click;
extern short mouse_x;
extern short mouse_y;
extern signed char text_colour;
extern int dungeon_water_level;
extern iptr big_buffer;
extern char str_loadsave_c[];
extern char str_invalid_object_data[];
extern char str_invalid_character_data[];
extern char str_image_raw[];
extern char str_fmt_s_s_4[];
extern char str_error_writing_to_drive[];
extern char str_savename_txt[];
extern char str_save[];
extern char str_wildcard[];
extern char str_rumor_dat_2[];
extern char str_bio_dat_2[];
extern char str_savetree_dat[];
extern char str_s0000021[];
extern char str_s0000021_qbn[];
extern char str_load00i0_img[];
extern char str_save00i0_img[];
extern char str_savex_image_raw[];
extern char str_savex_savename_txt[];
extern char saveload_prompt_fmt[];
extern char str_savevars_dat[];
extern char str_invalid_quest_object[];
extern unsigned char player_environment;
extern char travel_options[];
extern char quest_object_serial[];
extern int guards_timer;
extern char savevars_version[];
extern iptr prompt_enter_description;
extern struct rect saveload_buttons[];
extern char savetree_version[];
extern int last_unloaded_location;
extern char saved_location_name[];
extern char saved_region_name[];
extern char bio_modifiers[];
extern char saved_positions[];
extern char savevars_0f7[];
extern struct region regions[62];
extern signed char text_buffer[];
extern signed char saveload_slot_digit;
extern signed char scratch_190d16;
extern signed char text_rsc_buffer[];
extern char enchant_text[];
extern char arena2_path[];
extern signed char game_state_flags;
extern signed char ui_mode_flags;
extern unsigned char world_state_flags;
extern signed char ui_dirty_flags;
extern signed char quest_global_states[];
extern signed char load_requeue_flag_a;
extern signed char load_requeue_flag_b;
extern signed char load_requeue_flag_c;
extern int last_skill_check_minutes;
extern char loan_collectors_next[];
extern int bank_ship_price;
extern int bank_house_price;
extern struct record *nonworld_root;
extern char frame_counter[];
extern struct record *logbook_object;
extern struct record *options_object;
extern int view_look_pitch;
extern char savevars_0e1[];
extern struct record *inventory_containers[9];
extern int spell_points_bonus;
extern struct record *quest_root;
extern struct record *bank_accounts;
extern char D_00195A08[];
extern int D_00195A0C;
extern struct quest_face quest_faces[10];
extern int strength_bonus_end;
extern int jump_velocity;
extern struct record *camera_object;
extern struct building *current_building;
extern struct record *player_entity;
extern struct record *player_object;
extern char savevars_4_bytes[];
extern int vertical_velocity;
extern struct record *location_object;
extern char cheat_flags[];
extern char clothing_gender_group[];
extern char clothing_gender_offset[];
extern int effects_last_minute;
extern struct image *D_00195B5C;
extern char shared_scratch[];
extern int hud_message_expiry[];
extern struct location *current_location;
extern struct character *player_character;
extern iptr cursor_arrow_image;
extern iptr window_image;
extern struct career *player_class;
extern int game_minutes;
extern struct settings *game_settings;
extern iptr hud_message_ptrs[];
extern int realtime_clock_tick;
extern char *scratch_buffer;
extern char breath_remaining[];
extern int location_grid_x;
extern int location_grid_z;
extern int sky_loaded_frame;
extern signed char climate_weathers[];
extern short spell_ready_cost;
extern char saved_player_object[];
extern char pending_quest_file[];
extern signed char in_knightly_order_hall;
extern char savevars_byte[];
extern signed char text_macro_imperial;
extern signed char current_region;
extern signed char weapon_active_hand;
extern signed char mouse_buttons_prev;
extern signed char player_underwater;
extern signed char in_dungeon_water;
extern signed char crime_current;
extern char people_witness_flags[];
extern signed char is_daytime;
extern signed char world_loading;
extern signed char night_sky_loaded;
extern signed char msgbox_input_result;
extern iptr quest_debug_data;
extern int daylight;
extern int save_file_handle;
extern struct record *load_relink_root;
extern int save_version;
extern char cfg_mapsave_file[];
extern int terrain_cell_at_player;
extern int climate_index;
extern int recall_anchor_location;
extern int recall_anchor_region;
extern int recall_anchor_building;
extern int recall_anchor_environment;
extern int return_region;
extern int return_environment;
extern struct record *inv_left_container;
extern int return_location;

extern int func_000641CD(struct record *);
extern int sound_play(int, struct record *, int);
extern int mem_block_size(iptr);
extern iptr disk_read_file(char *, iptr);
extern int savetree_read_chunk(struct record *);
extern int savetree_write_record(struct record *);
extern int load_game(iptr);
extern int key_pressed_once(unsigned char);
extern struct record *object_delete(struct record *);
extern struct record *object_create_child(struct record *, struct record *, int);
extern struct record *object_reparent(struct record *, struct record *);
extern struct record *object_find_by_id(struct record *, iptr);
extern int object_new_id(int);
extern int xn_anim_reset(void *);
extern void xn_gfx_present_inclusive(int);
extern void xn_cam_set_focal(int, int);
extern void xn_cam_set_view_window(int, int, int, int);
extern int xn_mouse_poll_clamped(void);
extern void xn_kbd_flush(void);
extern void xn_draw_image_transparent(int, int, int, int, char *);
extern void faction_save(int);
extern void faction_load(int);
extern void region_locations_save_discovered(int);
extern void automap_save(void);
extern void quests_unlink_all(struct record *);
extern void quests_relink_all(struct record *);
extern void msgbox_show_string(iptr, int);
extern void fatal_error(char *);
extern void text_draw(iptr, int, int);
extern void text_draw_centred(iptr, int, int);
extern void links_save(int);
extern void mem_check_now(int);
extern void disk_copy_file(char *, char *, char *);
extern void disk_delete_matching(char *, char *);
extern void save_copy_automap_files(int);
extern void saveload_draw(int, int, int);
extern void text_draw_centred_coloured(char *, int, int, int, int);
extern void inpstr_begin_text(char *, int);
extern void object_foreach_pre(struct record *, void (*)());
extern void object_foreach_post(struct record *, void (*)());
extern void object_foreach(struct record *, void (*)());
struct record *savetree_attach_record(struct record *, struct record *, int);
int savetree_should_save(struct record *);
int save_write_name(char *);
int save_game(int, char *);
int saveload_click_slot(int, int, int, int);
int saveload_confirm(int, int, int, int);
void savetree_write_subtree(struct record *);
void savetree_write_saved(struct record *);
void savetree_register_record(struct record *);
void load_relink_object_cb(struct record *);
void load_relink_character(struct record *);
void load_collect_spawned_ids_cb(struct record *);
void load_drop_spawned_marker_cb(struct record *);
void save_write_image(void);
void savevars_write(int);
void load_fix_object_cb(struct record *);
void load_fix_ids_cb(struct record *);
#pragma aux mc_set_location parm routine [];

void savetree_read_records(struct record *root)
{
    struct record *record;
    struct record *object;
    int id;
    int size;

    record = (struct record *)scratch_buffer;
    size = savetree_read_chunk(record);
    while (size != 0) {
        record->prev = 0;
        record->next = record->prev;
        record->parent = record->next;
        record->children = record->parent;
        object = savetree_attach_record(root, record, size);
        savetree_register_record(object);
        id = object->id;
        size = savetree_read_chunk(record);
    }
}

void savetree_write_subtree(struct record *object)
{
    struct record *next;

    next = object->next;
    object->next = 0;
    object_foreach_pre(object, savetree_write_record);
    object->next = next;
}

void savetree_write_saved(struct record *object)
{
    int unused2;
    int unused3;
    int unused;

    unused = 0;
    while (object != 0) {
        if (savetree_should_save(object) != 0) {
            savetree_write_subtree(object);
        } else {
            savetree_write_saved(object->children);
        }
        object = object->next;
    }
}

struct record *savetree_attach_record(struct record *root, struct record *record, int size)
{
    struct record *parent;
    struct record *object;

    parent = object_find_by_id(root, record->parent_id);
    if (parent != 0) {
        object = object_find_by_id(root, record->id);
        if (object != 0 && object->type == record->type && object->parent->id == record->parent_id) {
            if (object->type == 34) {
                if ((object->image & 127) != (record->image & 127)) {
                    object = object_create_child(parent, record, size - RECORD_HEADER_SIZE);
                    object->parent_id = 0;
                    return object;
                }
            }
            mc_memcpy(object, record, RECORD_LINKS_OFFSET, str_loadsave_c, 217, 4);
            mc_memcpy(&object->data, &record->data, (int)(iptr)&*(signed char *)((char *)(iptr)mem_block_size((iptr)object) - RECORD_HEADER_SIZE), str_loadsave_c, 218, 4);
            object->parent_id = 0;
            return object;
        }
        object = object_create_child(parent, record, size - RECORD_HEADER_SIZE);
        object->parent_id = 0;
        return object;
    }
    fatal_error(str_invalid_object_data);
    return 0;
}

int savetree_should_save(struct record *object)
{
    if (object->quest_id != 0) return 1;
    if (func_000641CD(object) != 0) return 1;
    switch (object->type) {
    case 8:
        return object->flags & 512;
    case 2:
    case 3:
    case 4:
    case 9:
    case 14:
    case 18:
    case 19:
    case 20:
    case 25:
    case 26:
    case 27:
    case 32:
    case 36:
    case 42:
    case 44:
    case 45:
    case 46:
    case 51:
    case 52:
    case 53:
    case 54:
        return 1;
    case 33:
        if (object->children != 0) return 1;
        if (object->pad19 != 0) return 1;
        break;
    case 34:
        if (((object->image & 31) - 2) == 14 || ((object->image & 31) - 2) == 13) return 1;
        break;
    default:
        return 0;
    }
    return 0;
}

void savetree_register_record(struct record *object)
{
    switch (object->type) {
        return;
    case 4:
        player_object = object;
        return;
    case 5:
        camera_object = object;
        return;
    case 3:
        player_class = &(player_character = &(player_entity = object)->data.character)->career;
        return;
    case 52:
        if (inventory_containers[object->image] == 0 || inventory_containers[object->image]->children == 0) {
            inventory_containers[object->image] = object;
        }
        return;
    case 16:
        quest_root = object;
        return;
    case 23:
        game_settings = (struct settings *)((*(iptr *)&options_object = (iptr)object) + RECORD_HEADER_SIZE);
        return;
    case 24:
        logbook_object = object;
        return;
    case 25:
        bank_accounts = object;
        return;
    case 39:
        nonworld_root = object;
    default:;
    }
}

void load_relink_object_cb(struct record *object)
{
    switch (object->type) {
case 3:
case 18:
case 44:
    load_relink_character(object);
    return;
case 9:
    object->caster = object_find_by_id(location_object, (iptr)object->caster);
default:;
}
}

void load_relink_all(void)
{
    object_foreach(location_object, load_relink_object_cb);
    object_foreach(nonworld_root, load_relink_object_cb);
    inv_left_container = inventory_containers[0];
}

void load_relink_character(struct record *object)
{
    struct character *character;
    struct career *career;
    struct monster_anim *anim;
    int i;

    character = &object->data.character;
    career = &character->career;
    anim = (struct monster_anim *)(career + 1);
    for (i = 0; i < 27; i++) {
        if (character->equipped[i] != 0) {
            character->equipped[i] = object_find_by_id(location_object, (iptr)character->equipped[i]);
        }
    }
    if (character->target != 0) {
        character->target = object_find_by_id(location_object, (iptr)character->target);
        if (character->target == 0) fatal_error(str_invalid_character_data);
    }
    if (object->type != 18) if (object->type != 44) return;
    xn_anim_reset(anim);
}

void save_unlink_character(struct record *object)
{
    struct character *character;
    int i;

    character = &object->data.character;
    for (i = 0; i < 27; i++) {
        if (character->equipped[i] != 0) {
            character->equipped[i] = (struct record *)(uptr)character->equipped[i]->id;
        }
    }
    if (character->target == 0) return;
    if (object_find_by_id(location_object, character->target->id) != 0) {
        character->target = (struct record *)(uptr)character->target->id;
        return;
    }
    character->target = 0;
}

void load_collect_spawned_ids_cb(struct record *object)
{
    switch (object->type) {
case 18:
case 33:
case 44:
    *(int *)((scratch_buffer + ((*(int *)shared_scratch)++ << 2))) = object->id;
default:;
}
}

void load_drop_spawned_marker_cb(struct record *object)
{
    int *ids;
    int i;

    if (object->type != 34 || object->type == 32) return;
    ids = (int *)scratch_buffer;
    for (i = 0; i < *(int *)shared_scratch; i++, ids++) {
        if (object->id == *ids) {
            object_delete(object);
            *ids = 0;
            return;
        }
    }
}

void load_drop_spawned_markers(void)
{
    *(int *)shared_scratch = 0;
    object_foreach(location_object->children, load_collect_spawned_ids_cb);
    object_foreach_post(location_object->children, load_drop_spawned_marker_cb);
}

void save_write_image(void)
{
    mc_set_location(494, str_loadsave_c);
    mc_sprintf(enchant_text, str_fmt_s_s_4, (iptr)text_buffer, (iptr)str_image_raw);
    unlink(enchant_text);
    if ((save_file_handle = open(enchant_text, 546, 384)) < 0) {
        fatal_error(str_error_writing_to_drive);
    }
    write(save_file_handle, (void *)(big_buffer + 24000), 4000);
    close(save_file_handle);
}

int save_write_name(char *name)
{
    mc_set_location(516, str_loadsave_c);
    mc_sprintf(enchant_text, str_fmt_s_s_4, (iptr)text_buffer, (iptr)str_savename_txt);
    unlink(enchant_text);
    save_file_handle = open(enchant_text, 546, 384);
    write(save_file_handle, name, 32);
    close(save_file_handle);
    return 0;
}

int save_game(int slot, char *name)
{
    int size;

    mem_check_now(1000);
    mc_set_location(540, str_loadsave_c);
    mc_sprintf((char *)text_buffer, str_save, slot);
    disk_delete_matching(text_buffer, str_wildcard);
    save_write_image();
    save_write_name(name);
    savevars_write(slot);
    region_locations_save_discovered((int)(unsigned char)current_region);
    automap_save();
    mc_set_location(550, str_loadsave_c);
    mc_sprintf((char *)text_buffer, str_save, slot);
    disk_copy_file(str_rumor_dat_2, arena2_path, text_buffer);
    disk_copy_file(str_bio_dat_2, arena2_path, text_buffer);
    disk_copy_file(cfg_mapsave_file, arena2_path, text_buffer);
    save_copy_automap_files(slot);
    mc_set_location(558, str_loadsave_c);
    mc_sprintf((char *)text_rsc_buffer, str_fmt_s_s_4, (iptr)text_buffer, (iptr)str_savetree_dat);
    unlink((char *)text_rsc_buffer);
    save_file_handle = open((char *)text_rsc_buffer, 546, 384);
    write(save_file_handle, savetree_version, 4);
    write(save_file_handle, &player_object->x, 12);
    write(save_file_handle, &location_object->image, 2);
    write(save_file_handle, &player_environment, 1);
    size = current_location->building_count * 26;
    write(save_file_handle, &size, 4);
    write(save_file_handle, current_location->buildings, size);
    size = 0;
    quests_unlink_all(quest_root);
    savetree_write_saved(location_object->children);
    write(save_file_handle, &size, 4);
    object_foreach_pre(nonworld_root->children, savetree_write_record);
    write(save_file_handle, &size, 4);
    quests_relink_all(quest_root);
    links_save(save_file_handle);
    close(save_file_handle);
    mouse_buttons = (mouse_buttons_prev = 0);
    xn_mouse_poll_clamped();
    while (mouse_buttons != 0) xn_mouse_poll_clamped();
    mem_check_now(1001);
    return 0;
}

void load_requeue_s0000021(void)
{
    struct record *object;
    struct quest *quest;
    int unused;
    int count;

    object = quest_root->children;
    unused = 0;
    count = 0;
    if (((int)(unsigned char)current_region) == 31) return;
    while (object != 0) {
        if (object->type == 14) {
            quest = &object->data.quest;
            if (stricmp(quest->name, str_s0000021) == 0) count++;
        }
        object = object->next;
    }
    if (count != 0 || load_requeue_flag_b == 0 || load_requeue_flag_c != 0) return;
    mc_strncpy(pending_quest_file, str_s0000021_qbn, 13, str_loadsave_c, 616);
    load_requeue_flag_a = 0;
}

void saveload_menu(int saving)
{
    int done;
    int i;
    int used_slots;
    int slot;
    int handle;
    iptr window;

    done = 0;
    used_slots = 0;
    slot = 0;
    window = (window_image = disk_read_file(str_load00i0_img, 0));
    *(iptr *)&D_00195B5C = disk_read_file(str_save00i0_img, 0);
    mc_memset((void *)scratch_buffer, 0, 256, str_loadsave_c, 862, 4);
    mc_strncpy((char *)text_buffer, str_savex_image_raw, 160, str_loadsave_c, 864);
    for (i = 0; i < 6; i++) {
        saveload_slot_digit = *(signed char *)&i + 48;
        handle = open((char *)text_buffer, 512);
        if (handle < 1) continue;
        read(handle, (*(char **)&big_buffer + (i * 4000)), 4000);
        close(handle);
        used_slots |= 1 << i;
    }
    mc_strncpy((char *)text_buffer, str_savex_savename_txt, 160, str_loadsave_c, 874);
    for (i = 0; i < 6; i++) {
        saveload_slot_digit = *(signed char *)&i + 48;
        *(signed char *)((scratch_buffer + (i << 5))) = 0;
        handle = open((char *)text_buffer, 512);
        if (handle < 1) continue;
        read(handle, (scratch_buffer + (i << 5)), 32);
        close(handle);
    }
    while (done == 0) {
        if (key_pressed_once(15) != 0) {
            slot++;
            if (slot == 6) slot = 0;
        }
        if (key_pressed_once(1) != 0) done = 1;
        saveload_draw(saving, used_slots, slot);
        mouse_buttons_prev = mouse_buttons;
        xn_mouse_poll_clamped();
        xn_draw_image_transparent((int)(short)mouse_x, (int)(short)mouse_y, 10, 10, (char *)cursor_arrow_image);
        if (key_pressed_once(28) != 0) {
            mouse_double_click = 1;
            slot = saveload_click_slot(saving, saving, slot, used_slots);
            mouse_double_click = 0;
        } else if (mouse_double_click != 0 || (mouse_buttons != 0 && mouse_buttons_prev == 0)) {
            for (i = 0; i < 14; i++) {
                if (mouse_x > saveload_buttons[i].x0 && mouse_x < saveload_buttons[i].x1 && mouse_y > saveload_buttons[i].y0 && mouse_y < saveload_buttons[i].y1) {
                    sound_play(203, player_object, 100);
                    slot = saveload_buttons[i].handler(i, saving, slot, used_slots);
                }
                if (slot == (-1)) break;
            }
        }
        xn_gfx_present_inclusive(1);
        if (slot == (-1)) done = 1;
    }
    if (window != 0 && window != (-1751672937)) {
        mc_free((void *)window, str_loadsave_c, 924);
        window = -1751672937;
    }
    if ((iptr)D_00195B5C == 0 || (iptr)D_00195B5C == (-1751672937)) return;
    mc_free((void *)D_00195B5C, str_loadsave_c, 925);
    *(iptr *)&D_00195B5C = -1751672937;
}

int saveload_click_slot(int button, int saving, int slot, int used_slots)
{
    if (mouse_double_click != 0) {
        saveload_confirm(button, saving, slot, used_slots);
        return -1;
    }
    if (button < 6) return button % 3;
    return (button % 3) + 3;
}

int saveload_confirm(int button, int saving, int slot, int used_slots)
{
    iptr prompt;

    if (saving != 0) {
        mc_strncpy((char *)text_rsc_buffer, (scratch_buffer + (slot << 5)), 2048, str_loadsave_c, 948);
        text_colour = 146;
        prompt = (iptr)scratch_buffer + 55000;
        mc_set_location(951, str_loadsave_c);
        mc_sprintf((char *)prompt, saveload_prompt_fmt, prompt_enter_description);
        *(signed char *)((char *)(strlen((char *)prompt) + prompt) + 1) = 0;
        xn_kbd_flush();
        inpstr_begin_text(text_rsc_buffer, 31);
        mouse_buttons = (mouse_buttons_prev = 0);
        msgbox_show_string(prompt, 2);
        if (((int)(unsigned char)msgbox_input_result) == 2) return 0;
        save_game(slot, (char *)text_rsc_buffer);
    } else {
        load_game(slot);
    }
    scratch_190d16 = 0;
    return -1;
}

int saveload_exit(int button, int saving, int slot, int used_slots)
{
    scratch_190d16 = 1;
    return -1;
}

void savevars_read(iptr slot)
{
    mc_set_location(1008, str_loadsave_c);
    mc_sprintf((char *)text_buffer, str_save, slot);
    mc_set_location(1009, str_loadsave_c);
    mc_sprintf((char *)text_rsc_buffer, str_fmt_s_s_4, (iptr)text_buffer, (iptr)str_savevars_dat);
    save_file_handle = open((char *)text_rsc_buffer, 512);
    read(save_file_handle, saved_positions, 48);
    read(save_file_handle, bio_modifiers, 64);
    read(save_file_handle, &view_look_pitch, 12);
    read(save_file_handle, &text_macro_imperial, 1);
    PORT_READ_QUEST_FACES(save_file_handle, quest_faces, 100);
    read(save_file_handle, savevars_0e1, 20);
    read(save_file_handle, travel_options, 2);
    read(save_file_handle, savevars_0f7, 512);
    read(save_file_handle, savevars_byte, 1);
    PORT_READ_HEADER(save_file_handle, saved_player_object, 71);
    read(save_file_handle, &spell_points_bonus, 4);
    read(save_file_handle, D_00195A08, 4);
    read(save_file_handle, &D_00195A0C, 4);
    read(save_file_handle, &strength_bonus_end, 4);
    read(save_file_handle, quest_global_states, 64);
    read(save_file_handle, &spell_ready_cost, 2);
    read(save_file_handle, &is_daytime, 1);
    read(save_file_handle, pending_quest_file, 13);
    read(save_file_handle, quest_object_serial, 4);
    read(save_file_handle, &crime_current, 1);
    read(save_file_handle, people_witness_flags, 1);
    read(save_file_handle, &player_underwater, 1);
    read(save_file_handle, &in_dungeon_water, 1);
    read(save_file_handle, &guards_timer, 4);
    read(save_file_handle, breath_remaining, 4);
    read(save_file_handle, &location_grid_x, 4);
    read(save_file_handle, &location_grid_z, 4);
    read(save_file_handle, climate_weathers, 6);
    read(save_file_handle, &game_state_flags, 8);
    read(save_file_handle, frame_counter, 4);
    read(save_file_handle, &game_minutes, 4);
    read(save_file_handle, &realtime_clock_tick, 4);
    read(save_file_handle, clothing_gender_group, 4);
    read(save_file_handle, clothing_gender_offset, 4);
    read(save_file_handle, &weapon_active_hand, 1);
    read(save_file_handle, regions, 4960);
    read(save_file_handle, &current_region, 1);
    read(save_file_handle, cheat_flags, 4);
    read(save_file_handle, &vertical_velocity, 4);
    read(save_file_handle, savevars_4_bytes, 4);
    read(save_file_handle, &jump_velocity, 4);
    read(save_file_handle, savevars_version, 2);
    read(save_file_handle, &bank_house_price, 4);
    read(save_file_handle, &bank_ship_price, 4);
    read(save_file_handle, saved_location_name, 32);
    read(save_file_handle, saved_region_name, 32);
    read(save_file_handle, &in_knightly_order_hall, 1);
    read(save_file_handle, loan_collectors_next, 4);
    read(save_file_handle, &last_skill_check_minutes, 4);
    read(save_file_handle, &climate_index, 4);
    read(save_file_handle, climate_weathers, 6);
    read(save_file_handle, &dungeon_water_level, 4);
    read(save_file_handle, &last_unloaded_location, 4);
    read(save_file_handle, &recall_anchor_environment, 4);
    read(save_file_handle, &recall_anchor_location, 4);
    read(save_file_handle, &recall_anchor_region, 4);
    read(save_file_handle, &recall_anchor_building, 4);
    if (save_version >= 294) {
        read(save_file_handle, &return_region, 4);
        read(save_file_handle, &return_environment, 4);
        read(save_file_handle, &return_location, 4);
    }
    faction_load(save_file_handle);
    close(save_file_handle);
}

void savevars_write(int slot)
{
    mc_set_location(1092, str_loadsave_c);
    mc_sprintf((char *)text_rsc_buffer, str_fmt_s_s_4, (iptr)text_buffer, (iptr)str_savevars_dat);
    unlink((char *)text_rsc_buffer);
    save_file_handle = open((char *)text_rsc_buffer, 546, 384);
    write(save_file_handle, saved_positions, 48);
    write(save_file_handle, bio_modifiers, 64);
    write(save_file_handle, &view_look_pitch, 12);
    write(save_file_handle, &text_macro_imperial, 1);
    PORT_WRITE_QUEST_FACES(save_file_handle, quest_faces, 100);
    write(save_file_handle, savevars_0e1, 20);
    write(save_file_handle, travel_options, 2);
    write(save_file_handle, savevars_0f7, 512);
    write(save_file_handle, savevars_byte, 1);
    PORT_WRITE_HEADER(save_file_handle, saved_player_object, 71);
    write(save_file_handle, &spell_points_bonus, 4);
    write(save_file_handle, D_00195A08, 4);
    write(save_file_handle, &D_00195A0C, 4);
    write(save_file_handle, &strength_bonus_end, 4);
    write(save_file_handle, quest_global_states, 64);
    write(save_file_handle, &spell_ready_cost, 2);
    write(save_file_handle, &is_daytime, 1);
    write(save_file_handle, pending_quest_file, 13);
    write(save_file_handle, quest_object_serial, 4);
    write(save_file_handle, &crime_current, 1);
    write(save_file_handle, people_witness_flags, 1);
    write(save_file_handle, &player_underwater, 1);
    write(save_file_handle, &in_dungeon_water, 1);
    write(save_file_handle, &guards_timer, 4);
    write(save_file_handle, breath_remaining, 4);
    write(save_file_handle, &location_grid_x, 4);
    write(save_file_handle, &location_grid_z, 4);
    write(save_file_handle, climate_weathers, 6);
    write(save_file_handle, &game_state_flags, 8);
    write(save_file_handle, frame_counter, 4);
    write(save_file_handle, &game_minutes, 4);
    write(save_file_handle, &realtime_clock_tick, 4);
    write(save_file_handle, clothing_gender_group, 4);
    write(save_file_handle, clothing_gender_offset, 4);
    write(save_file_handle, &weapon_active_hand, 1);
    write(save_file_handle, regions, 4960);
    write(save_file_handle, &current_region, 1);
    write(save_file_handle, cheat_flags, 4);
    write(save_file_handle, &vertical_velocity, 4);
    write(save_file_handle, savevars_4_bytes, 4);
    write(save_file_handle, &jump_velocity, 4);
    write(save_file_handle, savevars_version, 2);
    write(save_file_handle, &bank_house_price, 4);
    write(save_file_handle, &bank_ship_price, 4);
    write(save_file_handle, saved_location_name, 32);
    write(save_file_handle, saved_region_name, 32);
    write(save_file_handle, &in_knightly_order_hall, 1);
    write(save_file_handle, loan_collectors_next, 4);
    write(save_file_handle, &last_skill_check_minutes, 4);
    write(save_file_handle, &climate_index, 4);
    write(save_file_handle, climate_weathers, 6);
    write(save_file_handle, &dungeon_water_level, 4);
    write(save_file_handle, &last_unloaded_location, 4);
    write(save_file_handle, &recall_anchor_environment, 4);
    write(save_file_handle, &recall_anchor_location, 4);
    write(save_file_handle, &recall_anchor_region, 4);
    write(save_file_handle, &recall_anchor_building, 4);
    write(save_file_handle, &return_region, 4);
    write(save_file_handle, &return_environment, 4);
    write(save_file_handle, &return_location, 4);
    faction_save(save_file_handle);
    close(save_file_handle);
}

void load_reset_state(void)
{
    int is_day;
    int minute_of_day;

    world_loading = 1;
    quest_debug_data = 0;
    terrain_cell_at_player = -1;
    if (((int)(unsigned short)(game_settings->view_flags & 1)) != 0) {
        xn_cam_set_view_window(160, 100, 160, 100);
    } else {
        xn_cam_set_view_window(160, 77, 160, 77);
    }
    xn_cam_set_focal(200, 180);
    sky_loaded_frame = 10000;
    night_sky_loaded = 0;
    mc_memset(hud_message_ptrs, 0, 68, str_loadsave_c, 1188, 68);
    mc_memset(hud_message_expiry, 0, 68, str_loadsave_c, 1189, 68);
    effects_last_minute = game_minutes;
    world_state_flags |= 1;
    ui_mode_flags |= 2;
    ui_dirty_flags |= 8;
    current_building = 0;
    minute_of_day = ((unsigned)game_minutes) % 1440;
    if (minute_of_day > 360 && minute_of_day < 1080) {
        is_day = 1;
    } else {
        is_day = 0;
    }
    daylight = is_day;
}

void load_fix_object_cb(struct record *object)
{
    char **model_ptr;
    struct block *block;
    struct block_model *model;
    int unused;
    int unused2;
    int i;
    int unused3;
    int unused4;
    {
        struct record *twin;

        if (object->twin != 0) {
            if (object->quest_id != 0) {
                twin = object_find_by_id(load_relink_root, (iptr)object->twin);
                object->twin = twin;
                if ((iptr)load_relink_root == (iptr)location_object) {
                    if (object->twin == 0) fatal_error(str_invalid_quest_object);
                    if (object->twin->twin == 0) object->twin->twin = (struct record *)object;
                }
            } else {
                object->twin = 0;
            }
        }
        switch (object->type) {
        case 52:
            if (object != inventory_containers[object->image]) {
                while (object->children != 0) {
                    object_reparent(inventory_containers[object->image], object->children);
                }
                object->type = 0;
            }
            return;
        case 33:
            if (object->parent->type == 52 && object->parent->image < 5) object->type = 2;
            return;
        case 43:
            block = &object->data.block;
            model = PORT_BLOCK_RELINK(block);
            for (i = 0; block->model_count > i; i++, model++) {
                model_ptr = &model->model;
                *model_ptr = 0;
            }
            return;
        case 56:
            model = (struct block_model *)RECORD_DATA(object);
            for (i = 0; object->model_count > i; i++, model++) {
                model_ptr = &model->model;
                *model_ptr = 0;
            }
            return;
        case 6:
        case 32:
            model_ptr = &object->data.instance.model;
            *model_ptr = 0;
        default:;
        }
    }
}

void load_fix_ids_cb(struct record *object)
{

    switch (object->type) {
    case 0:
    case 42:
        object_delete(object);
        return;
    case 9:
        if (object->parent->type == 47 || object->parent->type == 38 || object->parent->type == 1) {
            {
                iptr spell;
                spell = (iptr)RECORD_DATA(object);
            }
            if ((object->flags & 8192) == 0) {
                object_delete(object);
                return;
            }
            if (object->caster == player_entity) {
                object_delete(object);
                return;
            }
            if ((((unsigned)object->id) >> 16) != (((unsigned)location_object->id) >> 16)) {
                object->id = object_new_id(((unsigned)location_object->id) >> 16);
            }
            return;
        }
        if ((((unsigned)object->id) >> 16) == 801) return;
        object->id = object_new_id(801);
    default:;
    }
}

void load_fix_objects(void)
{
    load_relink_root = nonworld_root;
    object_foreach(location_object, load_fix_object_cb);
    load_relink_root = location_object;
    object_foreach(nonworld_root, load_fix_object_cb);
    object_foreach_post(location_object, load_fix_ids_cb);
}

void text_draw_centred_black_shadow(int text, int x, int y)
{
    int colour;

    *(short *)&colour = (int)(unsigned char)text_colour;
    text_colour = 0;
    text_draw_centred(text, (int)(iptr)&*(signed char *)((char *)(iptr)((int)(short)*(short *)&x) + 1), (int)(iptr)&*(signed char *)((char *)(iptr)((int)(short)*(short *)&y) + 1));
    text_colour = *(signed char *)&colour;
    text_draw_centred(text, (int)(short)*(short *)&x, (int)(short)*(short *)&y);
}

void text_draw_colour15_shadow(int text, int x, int y)
{
    int colour;

    *(short *)&colour = (int)(unsigned char)text_colour;
    text_colour = 15;
    text_draw(text, (int)(iptr)&*(signed char *)((char *)(iptr)((int)(short)*(short *)&x) + 1), (int)(iptr)&*(signed char *)((char *)(iptr)((int)(short)*(short *)&y) + 1));
    text_colour = *(signed char *)&colour;
    text_draw(text, (int)(short)*(short *)&x, (int)(short)*(short *)&y);
}

void text_draw_black_shadow(int text, int x, int y)
{
    int colour;

    *(short *)&colour = (int)(unsigned char)text_colour;
    text_colour = 0;
    text_draw(text, (int)(iptr)&*(signed char *)((char *)(iptr)((int)(short)*(short *)&x) + 1), (int)(iptr)&*(signed char *)((char *)(iptr)((int)(short)*(short *)&y) + 1));
    text_colour = *(signed char *)&colour;
    text_draw(text, (int)(short)*(short *)&x, (int)(short)*(short *)&y);
}

void text_draw_number_in_box(short x0, short y0, short x1, short y1, short number, short colour, short shadow_colour)
{
    char digits[12];

    text_draw_centred_coloured(itoa((int)(short)number, digits, 10), (int)(short)((((int)(short)x0) + ((int)(short)x1)) >> 1), (int)(short)(((((int)(short)y0) + ((int)(short)y1)) >> 1) - 2), (int)(short)colour, (int)(short)shadow_colour);
}
