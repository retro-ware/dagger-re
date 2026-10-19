/* support.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"

extern struct record *detect_target;
extern char shared_scratch[];


void detect_consider_creature(struct record *creature, int distance)
{
    creature->detect_distance = distance;
    if (distance >= *(int *)shared_scratch || distance >= 2048) return;
    detect_target = creature;
    *(int *)shared_scratch = distance;
}
