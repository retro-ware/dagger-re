/* matched by the real Watcom C32 10.0a (-d2): a run of click.c from 0x00077960 to 0x00077B9E, kept together for its switch table's alignment */
#include "records.h"
#include "clib.h"
extern char str_fm_sqr_2_hmi[];
extern char str_gshop_hmi[];
extern char str_gmage_3_hmi[];
extern char str_fm_nite3_hmi[];
extern char str_magic_2_hmi[];
extern char str_23_hmi[];
extern char str_23fm_hmi[];
extern char str_17_hmi[];
extern char str_17fm_hmi[];
extern char str_06_hmi[];
extern char str_06fm_hmi[];
extern char music_guild_factions_good[];
extern char music_guild_factions_bad[];
extern unsigned char player_environment;
extern char world_running;
extern struct building *current_building;
extern struct record *player_object;
extern struct record *location_object;
extern struct location *current_location;
extern unsigned int game_minutes;
extern int trespassing;
extern unsigned char climate_weathers[];
extern unsigned short building_music_seed;
extern char in_knightly_order_hall;
extern unsigned char current_region;
extern char is_daytime;
extern char D_001962A1;
extern char music_uses_fm;
extern char **music_court;
extern char **music_guild;
extern char **music_general;
extern char **music_sneaking;
extern char **music_dungeon;
extern char **music_buildings;
extern int climate_category(void);
extern void music_play(char *);
extern char *music_dungeon_song(void);
extern int location_contains(int, int);
extern int head_bob_offsets[];
extern char *str_0c[];
extern char *music_general_gm[];
extern char *music_sneaking_gm[];
extern char *music_guild_gm[];
extern char *music_buildings_gm[];
extern char *str_e[];
extern char *str_e_2[];
extern char *music_general_fm[];
extern char *music_sneaking_fm[];
extern char *music_guild_fm[];
extern char *music_buildings_fm[];
extern char *music_court_fm[];
extern int player_motion_flags;
extern int frame_ticks;
extern struct character *player_character;
extern struct settings *game_settings;
extern int head_bob_offset;
extern char in_dungeon_water;
extern char player_moving;
extern int head_bob_time;
extern int head_bob_last_time;
extern int spawn_point_wilderness(struct record *);
extern int spawn_point_town(struct record *, int, int);
extern int spawn_point_building(struct record *);
extern int spawn_point_dungeon(struct record *, int, int);

int spawn_find_point(struct record *object, int min_distance, int max_distance)
{
    int r;

    switch (player_environment) {
    case 1:
        if (location_object->image == 0xffff)
            r = spawn_point_wilderness(object);
        else
            r = spawn_point_town(object, min_distance, max_distance);
        break;
    case 2:
        r = spawn_point_building(object);
        break;
    case 3:
        r = spawn_point_dungeon(object, min_distance, max_distance);
        break;
    }
    if (r == 0)
        object->x = object->y = object->z = 0;
    return r;
}

void head_bob_update(void)
{
    if (!(game_settings->view_flags & 2) || (player_motion_flags & 0x20) || in_dungeon_water || (player_character->conditions & 8)) {
        head_bob_offset = 0;
        return;
    }
    if (player_moving)
        head_bob_time += frame_ticks;
    head_bob_time = head_bob_time % 1000;
    if (head_bob_last_time >= head_bob_time && !player_moving)
        head_bob_time = 0;
    head_bob_last_time = head_bob_time;
    head_bob_offset = head_bob_offsets[head_bob_time / 100];
}

void music_select_tables(void)
{
    if (music_uses_fm) {
        music_dungeon = str_e_2;
        music_general = music_general_fm;
        music_sneaking = music_sneaking_fm;
        music_guild = music_guild_fm;
        music_buildings = music_buildings_fm;
        music_court = music_court_fm;
        return;
    }
    music_dungeon = str_0c;
    music_general = music_general_gm;
    music_sneaking = music_sneaking_gm;
    music_guild = music_guild_gm;
    music_buildings = music_buildings_gm;
    music_court = str_e;
}

void music_choose_song(void)
{
    int seed;
    int climate;
    int idx;
    char *p;

    if (world_running == 0)
        return;
    seed = rand();
    if (player_environment == 3) {
        if (D_001962A1) {
            music_play(music_general[10]);
        } else {
            p = music_dungeon_song();
            if (p) {
                music_play(p);
            } else {
                srand((current_region << 8) ^ location_object->image);
                music_play(music_dungeon[rand() % 15]);
            }
        }
    } else if (player_environment == 1) {
        climate = climate_category();
        srand(game_minutes / 1440);
        if (is_daytime == 0) {
            music_play(music_court[rand() % 7]);
        } else if (!location_contains(player_object->x, player_object->z) || (current_location->kind != 4 && current_location->kind <= 9 ? 1 : 0)) {
            switch (climate_weathers[climate]) {
            case 0:
                music_play(music_general[rand() % 7]);
                break;
            case 1:
                music_play(music_general[rand() % 9]);
                break;
            case 2:
            case 3:
            case 6:
                music_play(music_general[rand() % 5 + 7]);
                break;
            case 4:
                music_play(music_general[rand() % 3 + 12]);
                break;
            case 5:
                music_play(music_general[rand() % 3 + 15]);
                break;
            }
        } else if (location_contains(player_object->x, player_object->z) && (current_location->kind == 4 || current_location->kind >= 9)) {
            music_play(music_court[rand() % 7]);
        } else {
            switch (climate_weathers[climate]) {
            case 0:
                music_play(music_general[rand() % 7]);
                break;
            case 1:
                music_play(music_general[rand() % 9]);
                break;
            case 2:
            case 3:
            case 6:
                music_play(music_general[rand() % 5 + 7]);
                break;
            case 4:
                music_play(music_general[rand() % 3 + 12]);
                break;
            case 5:
                music_play(music_general[rand() % 3 + 15]);
                break;
            }
        }
    } else {
        srand(building_music_seed);
        if (trespassing) {
            music_play(music_sneaking[rand() % 7]);
            srand(seed);
            return;
        }
        switch (current_building->type) {
        case 0:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 12:
        case 13:
            music_play(music_uses_fm == 0 ? str_gshop_hmi : str_fm_sqr_2_hmi);
            break;
        case 11:
            if (current_building->faction_id == 40) {
                if (rand() & 1)
                    music_play(music_uses_fm == 0 ? str_gmage_3_hmi : str_fm_nite3_hmi);
                else
                    music_play(music_uses_fm == 0 ? str_magic_2_hmi : str_fm_nite3_hmi);
            } else {
                music_play(music_uses_fm == 0 ? str_23_hmi : str_23fm_hmi);
            }
            break;
        case 14:
            if (in_knightly_order_hall) {
                music_play(music_uses_fm == 0 ? str_17_hmi : str_17fm_hmi);
            } else {
                p = memchr(music_guild_factions_good, current_building->faction_id, 8);
                idx = (int)(p - music_guild_factions_good);
                if (p == 0) {
                    p = memchr(music_guild_factions_bad, current_building->faction_id, 8);
                    if (p == 0) {
                        music_play(music_uses_fm == 0 ? str_17_hmi : str_17fm_hmi);
                        break;
                    }
                    idx = (int)(p - music_guild_factions_bad);
                }
                music_play(music_guild[idx]);
            }
            break;
        case 15:
            music_play(music_buildings[game_minutes / 1440 % 5]);
            break;
        case 16:
            /* a random pick from one choice: the code generator folds `% 1` to 0 and drops the
             * then-branch, but its ?: temp keeps the frame slot at [ebp-0x50] */
            if (rand() % 1)
                music_play(music_uses_fm == 0 ? str_06_hmi : str_06fm_hmi);
            else
                music_play(music_uses_fm == 0 ? str_06_hmi : str_06fm_hmi);
            break;
        default:
            music_play(music_uses_fm == 0 ? str_23_hmi : str_23fm_hmi);
            break;
        }
    }
    srand(seed);
}
