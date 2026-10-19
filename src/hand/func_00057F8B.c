/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00057F8B */
#include "records.h"
#include "clib.h"
extern char str_no_room_in_item[];
extern char enchant_power_excludes_type1[];
extern char enchant_power_excludes_param1[];
extern char enchant_power_excludes_type2[];
extern char enchant_power_excludes_param2[];
extern char enchant_power_exclusion_rows[];
extern char enchant_soul_power_codes[];
extern char enchant_soul_side_effect_codes[];
extern char enchant_side_effect_excludes_type1[];
extern char enchant_side_effect_excludes_param1[];
extern char enchant_side_effect_excludes_type2[];
extern char enchant_side_effect_excludes_param2[];
extern char enchant_side_effect_exclusion_rows[];
extern char enchant_soul_powers_type[];
extern char enchant_soul_powers_param[];
extern unsigned char enchant_soul_codes_kind[];
extern unsigned char enchant_soul_codes_list[];
extern char enchant_soul_codes_kind2[];
extern char enchant_soul_codes_list2[];
extern char enchant_soul_creatures[];
extern char *enchant_soul_code_lists[];
extern signed char scratch_190ce4[];
extern char scratch_190d64[];
extern signed char itemmaker_slot_exclusions_2[];
extern signed char itemmaker_slot_exclusions_3[];
extern signed char itemmaker_slot_exclusions_6[];
extern signed char itemmaker_slot_exclusions_7[];
extern signed char itemmaker_slot_exclusions_8[];
extern signed char itemmaker_slot_exclusions_9[];
extern char itemmaker_soul_codes[];
extern char itemmaker_soul_codes_code[];
extern char itemmaker_soul_side_codes[];
extern char itemmaker_soul_side_codes_code[];
extern struct enchantment itemmaker_slots[];
extern signed char itemmaker_slot_from_soul[];
extern void msgbox_show_string(char *, short);
extern void itemmaker_set_exclusions(short, short, short, short, short, short, short);
extern int itemmaker_free_slot(void);
extern int itemmaker_free_slot_count(void);

void itemmaker_add_soul_powers(int soul)
{
    int soul_row;
    int count;
    int i;
    int j;
    int type;
    int param;
    int slot;
    char *found;

    found = memchr(enchant_soul_creatures, soul, 12);
    if (found == 0) return;
    soul_row = (int)(found - enchant_soul_creatures);
    for (i = 0; i < 4; i += 2) {
        if (enchant_soul_codes_kind[soul_row * 4 + i] == 128) {
            for (j = 0; j < 5; j++) {
                found = memchr(enchant_soul_power_codes, ((unsigned char *)enchant_soul_code_lists[enchant_soul_codes_list[soul_row * 4 + i]])[j], 39);
                if (found == 0)
                    continue;
                itemmaker_soul_codes[j * 2] = 0;
                itemmaker_soul_codes_code[j * 2] = found - enchant_soul_power_codes;
            }
        } else if (enchant_soul_codes_kind[soul_row * 4 + i] == 129) {
            for (j = 0; j < 5; j++) {
                found = memchr(enchant_soul_side_effect_codes, ((unsigned char *)enchant_soul_code_lists[enchant_soul_codes_list[soul_row * 4 + i]])[j], 26);
                if (found == 0)
                    continue;
                itemmaker_soul_side_codes[j * 2] = 1;
                itemmaker_soul_side_codes_code[j * 2] = found - enchant_soul_side_effect_codes;
            }
        } else if (enchant_soul_codes_kind[soul_row * 4 + i] == 255)
            continue;
        ((char (*)[10])((char *)itemmaker_slot_exclusions_2))[*(short *)scratch_190d64][i * 2] = ((char *)enchant_soul_codes_kind)[soul_row * 4 + i];
        ((char (*)[10])((char *)itemmaker_slot_exclusions_3))[*(short *)scratch_190d64][i * 2] = ((char *)enchant_soul_codes_list)[soul_row * 4 + i];
    }
    count = 0;
    while (*(short *)(enchant_soul_powers_type + soul_row * 20 + count * 4) != -1)
        count++;
    if (count > 5)
        count = 5;
    if (itemmaker_free_slot_count() < count) {
        msgbox_show_string(str_no_room_in_item, 1);
        return;
    }
    for (i = 0; i < count; i++) {
        slot = itemmaker_free_slot();
        ((char *)itemmaker_slot_from_soul)[slot] = 1;
        itemmaker_slots[slot].type = *(short *)(enchant_soul_powers_type + soul_row * 20 + i * 4);
        itemmaker_slots[slot].param = *(short *)(enchant_soul_powers_param + soul_row * 20 + i * 4);
        type = *(short *)(enchant_soul_powers_type + soul_row * 20 + i * 4);
        param = *(short *)(enchant_soul_powers_param + soul_row * 20 + i * 4);
        if (*(short *)(enchant_soul_powers_type + soul_row * 20 + i * 4) < 15) {
            ((char *)scratch_190ce4)[slot] = 0;
            j = *(unsigned char *)(enchant_power_exclusion_rows + type);
            if (j == 0)
                itemmaker_set_exclusions(slot, type, param, -1, -1, -1, -1);
            else {
                j--;
                itemmaker_set_exclusions(slot, type, param, *(unsigned char *)(enchant_power_excludes_type1 + j * 20 + param * 4), *(unsigned char *)(enchant_power_excludes_param1 + j * 20 + param * 4), *(unsigned char *)(enchant_power_excludes_type2 + j * 20 + param * 4), *(unsigned char *)(enchant_power_excludes_param2 + j * 20 + param * 4));
            }
        } else {
            ((char *)scratch_190ce4)[slot] = 1;
            itemmaker_slots[slot].type -= 15;
            j = *(unsigned char *)(enchant_side_effect_exclusion_rows + type);
            if (j == 0)
                itemmaker_set_exclusions(slot, type, param, -1, -1, -1, -1);
            else {
                j--;
                itemmaker_set_exclusions(slot, type, param, *(unsigned char *)(enchant_side_effect_excludes_type1 + j * 20 + param * 4), *(unsigned char *)(enchant_side_effect_excludes_param1 + j * 20 + param * 4), *(unsigned char *)(enchant_side_effect_excludes_type2 + j * 20 + param * 4), *(unsigned char *)(enchant_side_effect_excludes_param2 + j * 20 + param * 4));
            }
        }
        ((char *)itemmaker_slot_exclusions_6)[slot * 10] = ((char *)enchant_soul_codes_kind)[soul_row * 4];
        ((char *)itemmaker_slot_exclusions_7)[slot * 10] = ((char *)enchant_soul_codes_list)[soul_row * 4];
        ((char *)itemmaker_slot_exclusions_8)[slot * 10] = enchant_soul_codes_kind2[soul_row * 4];
        ((char *)itemmaker_slot_exclusions_9)[slot * 10] = enchant_soul_codes_list2[soul_row * 4];
    }
}
