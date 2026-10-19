/* steal.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"

extern signed char text_colour;
extern char str_steal_c[];
extern short msgbox_wrap_width;
extern iptr text_lock_trivial;
extern iptr text_lock_miracle;
extern iptr text_lock_beyond;
extern iptr text_lock_difficulty[];
extern iptr text_lock_magic;
extern iptr text_pickpocket_failed;
extern iptr text_pinched_gold_fmt;
extern signed char text_buffer[];
extern char scratch_190be4[];
extern struct record *found_object;
extern struct character *player_character;
extern signed char crime_current;

extern iptr hud_message_add(iptr);
extern void monster_wake_all(void);
extern void skill_add_uses(int, int);
extern void msgbox_show_string(char *, short);
extern void msgbox_show_rsc(int, int);
extern void guards_summon(int);
extern void guild_count_crime(int, int);
extern void hud_status_set(iptr);
#pragma aux mc_set_location parm routine [];

void pickpocket_attempt(struct record *target)
{
    int gold;
    int chance;
    struct character *victim;

    chance = (int)(short)player_character->skills[15].value;
    skill_add_uses(15, 1);
    if (target->type == 18) {
        victim = &target->data.character;
        chance += (player_character->level - victim->level) * 5;
    }
    if (chance < 5) {
        chance = 5;
    } else if (chance > 95) {
        chance = 95;
    }
    if ((rand() % 101) > chance) {
        if (target->type == 18) monster_wake_all();
        hud_message_add(text_pickpocket_failed);
        crime_current = 12;
        guards_summon(1);
        if (target->type == 53) target->npc_flags |= 0x8000;
        return;
    }
    if ((rand() % 101) < 33) {
        msgbox_show_rsc(8999, 1);
        return;
    }
    msgbox_wrap_width = 250;
    text_colour = 145;
    gold = (rand() % 5) + 1;
    player_character->gold += gold;
    mc_set_location(155, str_steal_c);
    mc_sprintf((char *)text_buffer, (char *)text_pinched_gold_fmt, gold);
    msgbox_show_string(text_buffer, 1);
    guild_count_crime(5, 1);
}

void lock_show_difficulty(int lock_level)
{
    int chance;

    if (lock_level >= 20) {
        hud_status_set(text_lock_magic);
        return;
    }
    chance = player_character->skills[13].value - (lock_level * 5);
    if (chance < 30) {
        hud_status_set(text_lock_trivial);
        return;
    }
    if (chance < 35) {
        hud_status_set(text_lock_miracle);
        return;
    }
    if (chance < 45) {
        hud_status_set(text_lock_beyond);
        return;
    }
    hud_status_set(text_lock_difficulty[((chance - 45) / 5)]);
}

void door_key_match_cb(struct record *object)
{
    struct item *item;

    if (object->type != 2) return;
    item = &object->data.item;
    if (item->group != 27 || item->index != 3) return;
    if (item->value != *(int *)scratch_190be4 || (short)item->message != *(short *)scratch_190be4) return;
    found_object = object;
}
