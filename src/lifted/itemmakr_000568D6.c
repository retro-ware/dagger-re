/* itemmakr.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"

extern struct enchantment itemmaker_slots[];
extern char enchant_power_excludes_type1[];
extern char enchant_power_excludes_param1[];
extern char enchant_power_excludes_type2[];
extern char enchant_power_excludes_param2[];
extern char enchant_power_exclusion_rows[];
extern char scratch_190d64[];
extern char scratch_190d66[];

extern void itemmaker_set_exclusions(short, short, short, short, short, short, short);

void itemmaker_set_power_param_cb(int param)
{
    slot16 exclusion_row;

    itemmaker_slots[(int)(short)*(short *)scratch_190d64].param = param;
    *(int *)&exclusion_row = (int)(unsigned char)*(signed char *)(enchant_power_exclusion_rows + ((int)(short)*(short *)scratch_190d66));
    if ((short)exclusion_row == 0) {
        itemmaker_set_exclusions((int)(short)*(short *)scratch_190d64, (int)(short)*(short *)scratch_190d66, (int)(short)*(short *)&param, -1, -1, -1, -1);
        return;
    }
    (*(int *)&exclusion_row)--;
    itemmaker_set_exclusions((int)(short)*(short *)scratch_190d64, (int)(short)*(short *)scratch_190d66, (int)(short)*(short *)&param, (int)(short)((int)(unsigned char)*(signed char *)(enchant_power_excludes_type1 + ((((int)(short)exclusion_row) * 20) + (((int)(short)*(short *)&param) << 2)))), (int)(short)((unsigned short)(unsigned char)*(signed char *)(enchant_power_excludes_param1 + ((((int)(short)exclusion_row) * 20) + (((int)(short)*(short *)&param) << 2)))), (int)(short)((unsigned short)(unsigned char)*(signed char *)(enchant_power_excludes_type2 + ((((int)(short)exclusion_row) * 20) + (((int)(short)*(short *)&param) << 2)))), (int)(short)((unsigned short)(unsigned char)*(signed char *)(enchant_power_excludes_param2 + ((((int)(short)exclusion_row) * 20) + (((int)(short)*(short *)&param) << 2)))));
}
