/* custom.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "structs.h"

extern struct rect classmaker_buttons[];
extern short classmaker_attribute_up_y0;
extern short classmaker_attribute_up_y1;
extern short classmaker_attribute_down_y0;
extern short classmaker_attribute_down_y1;
extern short scratch_190d6a;
extern short chargen_selected_attribute;


void classmaker_select_attribute(slot16 attribute)
{
    chargen_selected_attribute = *(int *)&attribute;
    classmaker_attribute_up_y0 = (scratch_190d6a = classmaker_buttons[((int)(short)attribute) + 18].y0 + 1);
    classmaker_attribute_up_y1 = scratch_190d6a + 6;
    classmaker_attribute_down_y0 = scratch_190d6a + 13;
    classmaker_attribute_down_y1 = scratch_190d6a + 19;
}
