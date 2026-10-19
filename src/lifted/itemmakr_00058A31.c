/* itemmakr.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"

extern struct enchantment itemmaker_slots[];

extern void itemmaker_swap_side_offsets(void);

int itemmaker_power_excluded(int power)
{
    int slot;

    itemmaker_swap_side_offsets();
    for (slot = 0; slot < 10; slot++) {
        if (power == 25 && (itemmaker_slots[slot].type) == 25 && (itemmaker_slots[slot].param) == 5) {
            itemmaker_swap_side_offsets();
            return 1;
        }
        if (power == 14 && (itemmaker_slots[slot].type) == 14 && (itemmaker_slots[slot].param) == 5) {
            itemmaker_swap_side_offsets();
            return 1;
        }
    }
    itemmaker_swap_side_offsets();
    return 0;
}
