/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000166C5 */
#include "records.h"

extern unsigned char player_environment;
extern signed char talk_hint_quest_id;
extern int talk_hint_x;
extern int talk_hint_z;
extern struct record *player_object;
extern struct record *location_object;
extern struct talk_where *talk_where_target;
extern char talk_regional_rows[];
extern short *talk_topics;
extern struct faction *talk_npc_own_faction;
extern int talk_answer_kind;
extern char talk_selected_row[];
extern signed char talk_topic_tab;
extern signed char talk_news_asked;
extern int talk_find_regional(int);
extern void town_map_note_building(struct record *, iptr);
extern int rand_range(int, int);
extern struct record *object_find_by_id(struct record *, iptr);
extern int xn_math_approx_dist2d(int, int, int, int);


int talk_hint_text_id(int variant)
{
    int unused1;
    int unused2;
    int quest_bit;
    int unused3;
    int unused4;
    int unused5;
    int unused6;
    struct record *building_object;

    unused1 = 0;
    if ((unsigned char)talk_topic_tab == 3) {}
    if (talk_answer_kind == 2) {
        if (talk_where_target->pad04 == 0) {
            if (talk_where_target->building == 0) {
                if (talk_find_regional(*(unsigned char *)(talk_regional_rows + *(int *)talk_selected_row)) != 0)
                    return 10;
                return 11;
            }
            building_object = object_find_by_id(location_object, talk_where_target->building->id);
            talk_hint_x = building_object->x;
            talk_hint_z = building_object->z;
            if (player_environment == 1 && (xn_math_approx_dist2d(building_object->x, building_object->z, player_object->x, player_object->z) < 2048 || rand_range(1, 100) <= 25)) {
                town_map_note_building(building_object, (iptr)talk_where_target->building);
                return 7332;
            }
            return 7333;
        }
    } else {
        if (talk_npc_own_faction->id != 806 && talk_npc_own_faction->id != 842)
            talk_news_asked = 1;
        if (talk_topics[*(int *)talk_selected_row * 3 + 2] != 0) {
            talk_hint_quest_id = talk_topics[*(int *)talk_selected_row * 3 + 2];
            quest_bit = 32768;
        } else
            quest_bit = 0;
        if (variant != 0 && talk_topics[*(int *)talk_selected_row * 3 + 1] != 0)
            return quest_bit | talk_topics[*(int *)talk_selected_row * 3 + 1];
        if (talk_topics[*(int *)talk_selected_row * 3] != 0)
            return quest_bit | talk_topics[*(int *)talk_selected_row * 3];
        return quest_bit | talk_topics[*(int *)talk_selected_row * 3 + 1];
    }
    return 100;
}
