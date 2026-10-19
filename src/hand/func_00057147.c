/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00057147 */
#include "structs.h"
extern struct magic_enchantment itemmaker_slot_exclusions[][5];   /* itemmaker_slot_exclusions */

void itemmaker_set_exclusions(short slot, short type, short param, short type2, short param2, short type3, short param3)
{
    itemmaker_slot_exclusions[slot][0].type = type;
    itemmaker_slot_exclusions[slot][0].param = param;
    itemmaker_slot_exclusions[slot][1].type = type2;
    itemmaker_slot_exclusions[slot][1].param = param2;
    itemmaker_slot_exclusions[slot][2].type = type3;
    itemmaker_slot_exclusions[slot][2].param = param3;
}
