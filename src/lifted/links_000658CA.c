/* links.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */

extern int link_axis_override_keys[];
extern int link_axis_override_axes[];


int link_model_axis_override(int id_hundreds, int id_rest)
{
    int model_id;
    int i;

    model_id = id_rest + (id_hundreds * 100);
    for (i = 0; i < 6; i++) {
        if (link_axis_override_keys[i] == model_id) return link_axis_override_axes[i];
    }
    return 0;
}
