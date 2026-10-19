/* names.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"

extern char str_namegen_dat[];
extern char str_names_c[];
extern char str_space[];
extern char str_residence[];
extern char str_fmt_s_s_11[];
extern char str_your_house[];
extern char str_house_for_sale[];
extern char str_the_bank_of[];
extern char str_palace[];
extern char str_city_wall[];
extern char str_the_dark_brotherhood[];
extern char str_the_thieves_guild[];
extern char str_the_residence[];
extern char **shop_name_last_words[];
extern char *shop_name_first_words[];
extern char *tavern_name_first_words[];
extern char *tavern_name_last_words[];
extern char *region_names[];
extern signed char region_name_races[];
extern iptr text_pcf_house;
extern signed char text_buffer[];
extern char inpstr_saved_text[];
extern signed char text_rsc_buffer[];
extern struct record *location_object;
extern struct character *player_character;
extern signed char current_region;
extern int namegen_part_offsets[];
extern char namegen_name[];
extern char namegen_part_name[];
extern char namegen_syllable[];
extern short namegen_file;
extern signed char input_digits_only;

extern iptr faction_find(short);
extern struct record *quest_find_site_for_building(struct building *);
extern int disk_open_data(char *);
extern struct membership *guild_find_membership_by_kind(unsigned char);
extern int rand_range(int, int);
extern iptr name_generate_seeded(unsigned char, unsigned char, int);
extern char *name_generate_surname(unsigned char, unsigned char);
extern int inpstr_edit(char *, short, short, short, short, short);
extern void xn_kbd_flush(void);
extern void parse_expand(iptr, iptr);
extern void parse_rsc_text(int, int, int);
extern void namegen_read_part(short, short);
iptr name_generate_first(unsigned char, unsigned char);
iptr str_list_random(char **);
#pragma aux mc_set_location parm routine [];

iptr name_generate(unsigned char bank, unsigned char female)
{
    char *surname;

    *(signed char *)namegen_name = 0;
    namegen_file = disk_open_data(str_namegen_dat);
    lseek((int)(short)namegen_file, ((int)(unsigned char)bank) * 48, 0);
    read((int)(short)namegen_file, namegen_part_offsets, 48);
    mc_strncpy(namegen_name, (char *)name_generate_first((int)(unsigned char)bank, (int)(unsigned char)female), 40, str_names_c, 90);
    surname = (char *)name_generate_surname((int)(unsigned char)bank, (int)(unsigned char)female);
    if (*surname != 0) {
        func_000A1054(namegen_name, str_space, str_names_c, 96, 40);
        func_000A1054(namegen_name, surname, str_names_c, 97, 40);
    }
    close((int)(short)namegen_file);
    return (iptr)namegen_name;
}

iptr name_generate_first(unsigned char bank, unsigned char female)
{
    slot16 part;

    {
        int default_part;
        int nord_part;
        switch ((unsigned char)bank) {
        case 1:
            namegen_read_part((int)(short)namegen_file, 0);
            mc_strncpy(namegen_part_name, namegen_syllable, 30, str_names_c, 156);
            namegen_read_part((int)(short)namegen_file, 1);
            func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 158, 30);
            namegen_read_part((int)(short)namegen_file, 2);
            func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 160, 30);
            if (female == 0 && (rand() % 100) < 75) {
                namegen_read_part((int)(short)namegen_file, 3);
                func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 165, 30);
            }
            if (female != 0) {
                namegen_read_part((int)(short)namegen_file, 4);
                func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 171, 30);
            }
            break;
        case 2:
            if (female != 0) {
                nord_part = 2;
            } else {
                nord_part = 0;
            }
            *(int *)&part = nord_part;
            namegen_read_part((int)(short)namegen_file, (int)(short)part);
            mc_strncpy(namegen_part_name, namegen_syllable, 30, str_names_c, 177);
            namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 1));
            func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 179, 30);
            break;
        case 8:
            *(int *)&part = 0;
            namegen_read_part((int)(short)namegen_file, (int)(short)part);
            mc_strncpy(namegen_part_name, namegen_syllable, 30, str_names_c, 184);
            if ((rand() % 50) < 25) {
                namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 1));
                func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 189, 30);
            }
            namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 2));
            func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 193, 30);
            break;
        case 9:
            *(int *)&part = 0;
            namegen_read_part((int)(short)namegen_file, (int)(short)part);
            mc_strncpy(namegen_part_name, namegen_syllable, 30, str_names_c, 199);
            if ((rand() % 50) < 25) {
                namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 1));
                func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 204, 30);
            }
            namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 2));
            func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 208, 30);
            if (female != 0) {
                namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 3));
                func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 213, 30);
            }
            break;
        case 10:
            *(int *)&part = 0;
            if (female == 0 && (rand() % 100) < 25) {
                namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 3));
                mc_strncpy(namegen_part_name, namegen_syllable, 30, str_names_c, 223);
                func_000A1054(namegen_part_name, str_space, str_names_c, 224, 30);
                namegen_read_part((int)(short)namegen_file, (int)(short)part);
                func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 226, 30);
            } else {
                namegen_read_part((int)(short)namegen_file, (int)(short)part);
                mc_strncpy(namegen_part_name, namegen_syllable, 30, str_names_c, 231);
            }
            namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 1));
            func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 235, 30);
            namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 2));
            func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 237, 30);
            break;
        default:
            if (female != 0) {
                default_part = 2;
            } else {
                default_part = 0;
            }
            *(int *)&part = default_part;
            namegen_read_part((int)(short)namegen_file, (int)(short)part);
            mc_strncpy(namegen_part_name, namegen_syllable, 30, str_names_c, 242);
            namegen_read_part((int)(short)namegen_file, (int)(short)(*(int *)&part + 1));
            func_000A1054(namegen_part_name, namegen_syllable, str_names_c, 244, 30);
        }
        return (iptr)namegen_part_name;
    }
}

iptr building_name(struct building *building)
{
    int saved_seed;
    int unused;
    struct faction *faction;
    char *resident_name;
    char *space;
    struct record *quest_object;

    if (building == 0) return (iptr)str_residence;
    saved_seed = rand();
    if (building->id == player_character->house) {
        parse_expand(text_pcf_house, (iptr)inpstr_saved_text);
        return (iptr)inpstr_saved_text;
    }
    if (((int)(unsigned short)(building->name_seed & 32768)) != 0) {
        parse_rsc_text(building->name_seed & 0x7FFF, 0, 0);
        return (iptr)text_rsc_buffer;
    }
    srand(building->name_seed);
    switch (building->type) {
    case 0:
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
        mc_set_location(308, str_names_c);
        mc_sprintf((char *)text_buffer, str_fmt_s_s_11, str_list_random(shop_name_first_words), str_list_random(shop_name_last_words[building->type]));
        parse_expand((iptr)text_buffer, (iptr)text_rsc_buffer);
        break;
    case 1:
        if (((location_object->id & -65536) + building->id) == player_character->house) {
            mc_strncpy((char *)text_rsc_buffer, str_your_house, 2048, str_names_c, 313);
        } else {
            mc_strncpy((char *)text_rsc_buffer, str_house_for_sale, 2048, str_names_c, 315);
        }
        break;
    case 3:
        mc_set_location(318, str_names_c);
        mc_sprintf((char *)text_rsc_buffer, str_the_bank_of, (iptr)region_names[((int)(unsigned char)current_region)]);
        break;
    case 11:
        faction = (struct faction *)faction_find((int)(short)building->faction_id);
        if (faction != 0) {
            mc_strncpy((char *)text_rsc_buffer, faction->name, 2048, str_names_c, 323);
        }
        break;
    case 14:
        faction = (struct faction *)faction_find((int)(short)building->faction_id);
        if (faction->child != 0) {
            mc_strncpy((char *)text_rsc_buffer, faction->child->name, 2048, str_names_c, 332);
        } else if (faction != 0) {
            mc_strncpy((char *)text_rsc_buffer, faction->name, 2048, str_names_c, 334);
        }
        break;
    case 15:
        mc_set_location(341, str_names_c);
        mc_sprintf((char *)text_rsc_buffer, str_fmt_s_s_11, str_list_random(tavern_name_first_words), str_list_random(tavern_name_last_words));
        break;
    case 16:
        mc_set_location(344, str_names_c);
        mc_sprintf((char *)text_rsc_buffer, str_palace);
        break;
    case 23:
        mc_strncpy((char *)text_rsc_buffer, str_city_wall, 2048, str_names_c, 347);
        break;
    default:
        if (building->faction_id == 108 && guild_find_membership_by_kind(0) != 0) {
            mc_strncpy((char *)text_rsc_buffer, str_the_dark_brotherhood, 2048, str_names_c, 351);
        } else if (building->faction_id == 42 && guild_find_membership_by_kind(3) != 0) {
            mc_strncpy((char *)text_rsc_buffer, str_the_thieves_guild, 2048, str_names_c, 353);
        } else {
            quest_object = quest_find_site_for_building(building);
            if (quest_object != 0) {
                if (quest_object != 0 && (quest_object->type == 41 || quest_object->type == 8)) {
                    resident_name = (char *)name_generate_seeded((int)(unsigned char)region_name_races[(int)(unsigned char)current_region], (int)(unsigned char)((signed char)quest_object->flags & 4), quest_object->name_seed);
                } else {
                    resident_name = (char *)name_generate_seeded((int)(unsigned char)region_name_races[(int)(unsigned char)current_region], 0, (((unsigned)building->id) >> 16) ^ building->id);
                }
                space = (char *)strchr(resident_name, 32);
                if (space != 0) resident_name = space + 1;
                mc_set_location(363, str_names_c);
                mc_sprintf((char *)text_rsc_buffer, str_the_residence, resident_name);
            } else {
                mc_strncpy((char *)text_rsc_buffer, str_residence, 2048, str_names_c, 366);
            }
        }
    }
    srand(saved_seed);
    return (iptr)text_rsc_buffer;
}

iptr str_list_random(char **list)
{
    int entry;

    entry = 0;
    while (list[entry++] != 0);
    entry = rand_range(0, entry - 2);
    return (iptr)list[entry];
}

void input_edit_number_box(char *text, short x, int y, short width, short height, short max_length)
{
    xn_kbd_flush();
    input_digits_only = 1;
    inpstr_edit(text, (int)(short)x, (int)(short)*(short *)&y, (int)(short)width, (int)(short)height, (int)(short)max_length);
    input_digits_only = 0;
}
