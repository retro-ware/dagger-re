/* parse.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "ptrint.h"

extern signed char region_name_races[];
extern signed char song_hero_race;

extern iptr name_generate_seeded(unsigned char, unsigned char, int);

iptr parse_regional_name(int seed, int gender)
{
    return name_generate_seeded((int)(unsigned char)region_name_races[(int)(signed char)song_hero_race], (int)(unsigned char)*(signed char *)&gender, seed);
}
