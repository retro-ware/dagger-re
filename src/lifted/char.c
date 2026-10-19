/* char.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"

extern char str_char_c[];
extern signed char region_name_races[];
extern signed char current_region;
extern char npc_record_buffer[];
extern char npc_talk_flags[];
extern signed char npc_talk_region_value;
extern short npc_talk_seeds[];
extern short npc_talk_faction;

extern struct faction *faction_find_type_in_region(short, short);
extern iptr faction_find(short);
extern iptr flats_cfg_find(int);
extern iptr npc_display_name(struct record *);

iptr npc_talk_record_build(struct record *npc)
{
    char *npc_data;
    int i;
    int saved_seed;
    struct flat_cfg *flat_cfg;
    struct faction *faction;

    saved_seed = rand();
    flat_cfg = (struct flat_cfg *)flats_cfg_find(npc->image);
    npc_data = RECORD_DATA(npc);
    mc_memset(npc_record_buffer, 0, REC_OFFSETOF(struct character, career), str_char_c, 226, 4);
    if (npc->type == 53) {
        *(short *)npc_talk_flags = ((int)(unsigned short)*(short *)npc_talk_flags) | ((((int)(unsigned short)(npc->npc_flags & 16384)) != 0) ? 1 : 0);
    } else if (flat_cfg != 0 && (flat_cfg->flags & 1) != 0) {
        *(signed char *)npc_talk_flags |= 1;
    } else if (flat_cfg == 0 && ((int)(unsigned char)(*(signed char *)(npc_data + 2) & 16)) != 0) {
        *(signed char *)npc_talk_flags |= 1;
    }
    npc_talk_region_value = region_name_races[(int)(unsigned char)current_region];
    if (npc->type == 8) {
        npc_talk_faction = *(short *)npc_data;
    } else {
        npc_talk_faction = faction_find_type_in_region((int)(short)((int)(unsigned char)current_region), 15)->id;
    }
    if (npc_talk_faction == 0) {
        npc_talk_faction = faction_find_type_in_region((int)(short)((int)(unsigned char)current_region), 15)->id;
    }
    faction = (struct faction *)faction_find((int)(short)npc_talk_faction);
    if (faction != 0 && faction->type == 4) {
        mc_strncpy(npc_record_buffer, faction->name, 32, str_char_c, 245);
    } else {
        mc_strncpy(npc_record_buffer, (char *)npc_display_name(npc), 32, str_char_c, 247);
    }
    srand(npc->id);
    for (i = 1; i < 5; i++) {
        npc_talk_seeds[i] = rand();
    }
    srand(saved_seed);
    return (iptr)npc_record_buffer;
}
