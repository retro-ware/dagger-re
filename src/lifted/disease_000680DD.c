/* disease.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */

#include "records.h"
#include "bitfield.h"
#include "clib.h"

extern struct record *player_entity;
extern struct record *scratch_current_object;
extern char effect_tick_minutes[];
extern char shared_scratch[];
extern struct character *player_character;
extern int game_minutes;
extern int nearest_creature_distance;
extern struct record *nearest_creature;
extern char extra_spell_points[];
extern int magicka_max_modifier;

extern int damage_apply(struct record *, int, struct record *);
extern int player_in_daylight(void);
extern int player_in_temple(void);
extern void item_damage(struct record *, int);
extern void enchant_extra_spell_points(struct item *, int);
extern void object_foreach(struct record *, void (*)());
extern void item_repair_cb(struct record *);
extern void item_break(struct record *);

void item_enchantment_tick(struct item *item, int enchant_type, int enchant_param)
{
    int unused;

    if (enchant_type != 3 && *(int *)effect_tick_minutes == 0) return;
    switch ((unsigned)enchant_type) {
    case 3:
        *(int *)extra_spell_points = 0;
        enchant_extra_spell_points(item, enchant_param);
        player_character->max_magicka += *(short *)extra_spell_points;
        magicka_max_modifier += *(int *)extra_spell_points;
        if (*(int *)extra_spell_points != 0 && enchant_param >= 7 && enchant_param <= 10 && player_character->magicka < player_character->max_magicka) {
            player_character->magicka += *(short *)effect_tick_minutes * 5;
        }
        if (*(int *)effect_tick_minutes != 0 && ((struct bf8_0_2 *)&game_minutes)->f == 0) {
            item_damage(scratch_current_object, 1);
        }
        break;
    case 5:
        switch ((unsigned)enchant_param) {
        case 1:
            if (player_in_daylight() == 0) return;
            goto L68256;
        case 2:
            if (player_in_daylight() != 0) return;
        default:
L68256:;
            player_character->health += *(short *)effect_tick_minutes;
            if (player_character->health > player_character->max_health) {
                player_character->health = player_character->max_health;
            } else if ((rand() % 10) == 0) {
                item_damage(scratch_current_object, 1);
            }
        }
        break;
    case 17:
        if ((enchant_param == 0 && player_in_daylight() != 0) || (enchant_param != 0 && player_in_temple() != 0)) {
            damage_apply(player_entity, *(int *)effect_tick_minutes, 0);
        }
        break;
    case 16:
        switch ((unsigned)enchant_param) {
        case 1:
            if (player_in_daylight() == 0) return;
            break;
        case 2:
            if (player_in_temple() == 0) return;
        }
        if (item->condition > *(int *)effect_tick_minutes) {
            item->condition -= *(short *)effect_tick_minutes;
        } else {
            item_break(scratch_current_object);
        }
        break;
    case 21:
        switch ((unsigned)enchant_param) {
        case 1:
            if (*(int *)effect_tick_minutes != 0 && ((unsigned)(game_minutes - player_character->last_kill_time)) > 1440) {
                damage_apply(player_entity, *(int *)effect_tick_minutes, 0);
            }
            break;
        case 2:
            if (*(int *)effect_tick_minutes != 0 && ((unsigned)(game_minutes - player_character->last_kill_time)) > 10080) {
                damage_apply(player_entity, *(int *)effect_tick_minutes, 0);
            }
        }
        break;
    case 8:
        *(int *)shared_scratch = *(int *)effect_tick_minutes;
        object_foreach(player_entity->children, item_repair_cb);
        if ((rand() % 10) == 0) item_damage(scratch_current_object, 1);
        break;
    case 1:
        item_damage(scratch_current_object, 1);
        break;
    case 6:
        if (enchant_param == 0 && nearest_creature_distance < 128 && player_character->health != player_character->max_health) {
            damage_apply(nearest_creature, *(int *)effect_tick_minutes, 0);
            player_character->health += *(short *)effect_tick_minutes;
            if (player_character->health > player_character->max_health) {
                player_character->health = player_character->max_health;
            }
            item_damage(scratch_current_object, *(int *)effect_tick_minutes);
        }
    }
    if (player_character->magicka >= 0) return;
    player_character->magicka = 0;
}
