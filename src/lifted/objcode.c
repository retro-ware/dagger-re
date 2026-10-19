/* objcode.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"
#include "doslow.h"

extern char str_objcode_c[];
extern char str_you_cannot_initiate_fast_travel_during[];
extern char str_trav0i00_img[];
extern char str_tele00i0_img[];
extern char str_trav0i04_img[];
extern char str_trav0i01_img[];
extern char str_trav01i0_img[];
extern char str_trav01i1_img[];
extern char str_mbrd00i0_img[];
extern char str_travai05_img[];
extern char str_travbi05_img[];
extern char str_travci05_img[];
extern char str_travdi05_img[];
extern char str_trav0i03_img[];
extern char str_fmap_pal_col[];
extern unsigned char player_environment;
extern char travel_options[];
extern iptr D_0018507B;
extern signed char world_running;
extern int scratch_190cac;
extern signed char scratch_190ce4[];
extern signed char scratch_190ce5;
extern signed char scratch_190ce7;
extern signed char D_001940D5;
extern struct record *location_grid[];
extern struct record *player_object;
extern struct record *location_object;
extern struct record *found_object;
extern int creature_count;
extern struct image *D_00195B5C;
extern char shared_scratch[];
extern struct character *player_character;
extern iptr window_image;
extern struct career *player_class;
extern char *scratch_buffer;
extern int location_grid_x;
extern int location_grid_z;
extern char found_marker[];
extern int D_00195F71;
extern int D_00195F75;
extern int D_00195F79;
extern short D_00195F81;
extern short D_00195F85;
extern int D_00195F89;
extern unsigned char D_0019626F;
extern signed char D_00196272;
extern signed char game_mode;
extern signed char is_daytime;
extern int marker_best;
extern int marker_kind;
extern struct image *travel_border_image_a;
extern struct image *travel_border_image_b;
extern struct image *travel_border_image_c;
extern struct image *travel_border_image_d;
extern char *travel_buttons_image[];
extern iptr travel_buttons_pressed_image;
extern struct image *travel_region_image;
extern iptr travel_region_index_image;
extern struct image *travel_map_border_image;
extern iptr travel_popup_image;
extern signed char travel_map_parts;
extern signed char travel_map_part;
extern signed char travel_map_mode;

extern iptr disk_read_file(char *, iptr);
extern int gold_can_afford(int);
extern int object_find_open(struct record *, iptr (*)());
extern int travel_trip_cost(void);
extern void xn_pal_set_all_8bit(char *);
extern int xn_math_isqrt(int);
extern void msgbox_show_string(iptr, int);
extern void object_foreach_open(struct record *, void (*)());
iptr marker_match_cb(struct record *);
struct record *marker_find_random(struct record *, int);
struct record *marker_find_nearest(struct record *, int);
iptr location_cell_at(int, int);
void marker_nearest_cb(struct record *);

iptr marker_match_cb(struct record *object)
{
    struct block *block;
    struct block_flat *flat;
    struct block_model *models;
    int unused;
    int i;

    switch (object->type) {
    case 34:
        if ((object->image >> 7) == 199 && ((object->image & 31) - 2) == marker_kind) {
            if (*(int *)shared_scratch == 0 && marker_best < 0) {
                mc_memcpy(found_marker, object, RECORD_LINKS_OFFSET, str_objcode_c, 191, 4);
                return 1;
            }
            *(int *)shared_scratch += marker_best;
        }
        break;
    case 43:
        block = &object->data.block;
        flat = block->flats;
        for (i = 0; block->flat_count > i; i++, flat++) {
            if ((flat->image >> 7) == 199) {
                if ((((int)(unsigned short)(flat->image & 31)) - 2) == marker_kind) {
                    if (*(int *)shared_scratch == 0 && marker_best < 0) {
                        D_00195F71 = flat->x;
                        D_00195F79 = flat->z;
                        D_00195F75 = flat->y;
                        D_00195F85 = flat->image;
                        D_00195F89 = object->id;
                        *(signed char *)found_marker = 34;
                        return 1;
                    }
                    *(int *)shared_scratch += marker_best;
                }
            }
        }
        break;
    case 56:
        models = (struct block_model *)RECORD_DATA(object);
        flat = (struct block_flat *)(models + object->model_count);
        for (i = 0; object->flat_count > i; i++, flat++) {
            if ((flat->image >> 7) == 199) {
                if ((((int)(unsigned short)(flat->image & 31)) - 2) == marker_kind) {
                    if (*(int *)shared_scratch == 0 && marker_best < 0) {
                        D_00195F71 = flat->x;
                        D_00195F79 = flat->z;
                        D_00195F75 = flat->y;
                        D_00195F85 = flat->image;
                        D_00195F89 = object->id;
                        *(signed char *)found_marker = 34;
                        return 1;
                    }
                    *(int *)shared_scratch += marker_best;
                }
            }
        }
    }
    return 0;
}

struct record *marker_find_first(struct record *root, int kind)
{
    marker_kind = kind;
    *(int *)shared_scratch = 0;
    marker_best = -1;
    mc_memset(found_marker, 0, RECORD_HEADER_SIZE, str_objcode_c, 261, 4);
    object_find_open(root, marker_match_cb);
    if (*(signed char *)found_marker != 0) return (struct record *)found_marker;
    return 0;
}

struct record *marker_find_nth(struct record *root, int kind, int n)
{
    iptr cell;

    marker_kind = kind;
    *(int *)shared_scratch = n;
    marker_best = -1;
    mc_memset(found_marker, 0, RECORD_HEADER_SIZE, str_objcode_c, 287, 4);
    object_find_open(root, marker_match_cb);
    cell = location_cell_at(D_00195F71, D_00195F79);
    D_00195F81 = n;
    if (*(signed char *)found_marker != 0) return (struct record *)found_marker;
    return 0;
}

struct record *marker_find_random(struct record *root, int kind)
{
    int n;

    marker_kind = kind;
    *(int *)shared_scratch = 0;
    marker_best = 1;
    mc_memset(found_marker, 0, RECORD_HEADER_SIZE, str_objcode_c, 317, 4);
    object_find_open(root, marker_match_cb);
    if (*(int *)shared_scratch == 0) return 0;
    n = (*(int *)shared_scratch = rand() % *(int *)shared_scratch);
    marker_best = -1;
    object_foreach_open(root, marker_match_cb);
    D_00195F81 = n;
    return (struct record *)found_marker;
}

void marker_nearest_cb(struct record *object)
{
    struct block *block;
    struct block_flat *flat;
    struct block_model *models;
    int dist;
    int i;

    switch (object->type) {
    case 34:
        if ((object->image >> 7) == 199 && ((object->image & 31) - 2) != marker_kind) return;
        dist = (object->x - player_object->x) * (object->x - player_object->x);
        dist += ((object->y - player_object->y) * (object->y - player_object->y)) * 2;
        dist += (object->z - player_object->z) * (object->z - player_object->z);
        if (dist != 0) dist = xn_math_isqrt(dist);
        if (dist < marker_best) {
            found_object = object;
            marker_best = dist;
            mc_memcpy(found_marker, object, RECORD_LINKS_OFFSET, str_objcode_c, 363, 4);
        }
        return;
    case 43:
        block = &object->data.block;
        flat = block->flats;
        for (i = 0; block->flat_count > i; i++, flat++) {
            if ((flat->image >> 7) == 199 && (((int)(unsigned short)(flat->image & 31)) - 2) == marker_kind) {
                dist = (flat->x - player_object->x) * (flat->x - player_object->x);
                dist += ((flat->y - player_object->y) * (flat->y - player_object->y)) * 2;
                dist += (flat->z - player_object->z) * (flat->z - player_object->z);
                if (dist != 0) dist = xn_math_isqrt(dist);
                if (dist < marker_best) {
                    marker_best = dist;
                    *(signed char *)found_marker = 34;
                    D_00195F71 = flat->x;
                    D_00195F79 = flat->z;
                    D_00195F75 = flat->y;
                    D_00195F85 = flat->image;
                    D_00195F89 = object->id;
                }
            }
        }
        return;
    case 56:
        models = (struct block_model *)RECORD_DATA(object);
        flat = (struct block_flat *)(models + object->model_count);
        for (i = 0; object->flat_count > i; i++, flat++) {
            if ((flat->image >> 7) == 199 && (((int)(unsigned short)(flat->image & 31)) - 2) == marker_kind) {
                dist = (flat->x - player_object->x) * (flat->x - player_object->x);
                dist += ((flat->y - player_object->y) * (flat->y - player_object->y)) * 2;
                dist += (flat->z - player_object->z) * (flat->z - player_object->z);
                if (dist != 0) dist = xn_math_isqrt(dist);
                if (dist < marker_best) {
                    marker_best = dist;
                    *(signed char *)found_marker = 34;
                    D_00195F71 = flat->x;
                    D_00195F79 = flat->z;
                    D_00195F75 = flat->y;
                    D_00195F85 = flat->image;
                    D_00195F89 = object->id;
                }
            }
        }
    default:;
    }
}

struct record *marker_find_nearest(struct record *root, int kind)
{
    marker_kind = kind;
    marker_best = 500000;
    mc_memset(found_marker, 0, RECORD_HEADER_SIZE, str_objcode_c, 432, 4);
    object_foreach_open(root, marker_nearest_cb);
    if (*(signed char *)found_marker != 0) return (struct record *)found_marker;
    return 0;
}

int marker_count(struct record *root, int kind)
{
    marker_kind = kind;
    *(int *)shared_scratch = 0;
    marker_best = 1;
    mc_memset(found_marker, 0, RECORD_HEADER_SIZE, str_objcode_c, 456, 4);
    object_find_open(root, marker_match_cb);
    return *(int *)shared_scratch;
}

int player_to_nearest_marker(struct record *root, int kind)
{
    struct record *marker;

    marker = marker_find_nearest(root, kind);
    if (marker != 0) {
        player_object->x = D_00195F71;
        player_object->y = D_00195F75;
        player_object->z = D_00195F79;
        D_001940D5 |= 2;
        return 1;
    }
    return 0;
}

int player_to_random_marker(struct record *root, int kind)
{
    struct record *marker;

    marker = marker_find_random(root, kind);
    if (marker != 0) {
        player_object->x = D_00195F71;
        player_object->y = D_00195F75;
        player_object->z = D_00195F79;
        D_001940D5 |= 2;
        return 1;
    }
    return 0;
}

iptr location_cell_at(int x, int z)
{
    int cell;

    x = ((x - location_object->x) + location_grid_x) / 1024;
    z = ((z - location_object->z) + location_grid_z) / 1024;
    cell = x + (z << 5);
    return (iptr)location_grid[cell];
}

int travel_map_open(int mode)
{
    pslot16 ticks_addr;

    if (((int)D_0019626F) == 19 && ((int)(unsigned char)game_mode) == 8) {
        return 1;
    }
    if (((int)player_environment) != 1) return 0;
    if (mode != 0) {
        if ((player_character->race == 8 || (player_class->flags & 16) != 0) && is_daytime != 0) {
            msgbox_show_string((iptr)str_you_cannot_initiate_fast_travel_during, 1);
            return 0;
        }
        if (creature_count != 0) {
            msgbox_show_string(D_0018507B, 1);
            return 0;
        }
        if (((int)(unsigned short)(*(short *)travel_options & 16)) != 0 && gold_can_afford(travel_trip_cost()) == 0) {
            *(signed char *)travel_options ^= 48;
        }
        if (((int)(unsigned short)(*(short *)travel_options & 8)) != 0 && player_character->ship_owned == 0 && gold_can_afford(travel_trip_cost()) == 0) {
            *(signed char *)travel_options ^= 12;
        }
        travel_map_mode = *(signed char *)&mode;
        window_image = disk_read_file(str_trav0i00_img, 0);
        if (mode == 100) {
            travel_popup_image = disk_read_file(str_tele00i0_img, 0);
        } else {
            travel_popup_image = disk_read_file(str_trav0i04_img, 0);
        }
        travel_region_index_image = disk_read_file(str_trav0i01_img, 0);
        *(iptr *)&travel_buttons_image[0] = disk_read_file(str_trav01i0_img, 0);
        travel_buttons_pressed_image = disk_read_file(str_trav01i1_img, 0);
        *(iptr *)&travel_map_border_image = disk_read_file(str_mbrd00i0_img, 0);
        *(iptr *)&travel_border_image_a = disk_read_file(str_travai05_img, 0);
        *(iptr *)&travel_border_image_b = disk_read_file(str_travbi05_img, 0);
        *(iptr *)&travel_border_image_c = disk_read_file(str_travci05_img, 0);
        *(iptr *)&travel_border_image_d = disk_read_file(str_travdi05_img, 0);
        *(iptr *)&D_00195B5C = disk_read_file(str_trav0i03_img, 0);
        disk_read_file(str_fmap_pal_col, (iptr)scratch_buffer);
        xn_pal_set_all_8bit((char *)((iptr)scratch_buffer + 8));
        *(iptr *)&ticks_addr = (iptr)DOS_LOW(0x46C);
        scratch_190cac = *(int *)(*(char **)&ticks_addr);
        travel_map_parts = (travel_map_part = 0);
        scratch_190ce5 = (scratch_190ce4[0] = 0);
        *(iptr *)&travel_region_image = 0;
        game_mode = 19;
        D_00196272 = 1;
        world_running = 0;
        scratch_190ce7 = 0;
    }
    return ((((int)(unsigned char)game_mode) == 19) ? 1 : 0);
}
