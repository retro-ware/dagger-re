/* talk.c */

#include "dagger.h"

void talk_prostitute_service(void) { }

int talk_stub_false(int unused) { return 0; }

int building_distance(char *building)
{
    char *object;
    object = object_find_by_id(location_object, *(int *)(building + 0x14));
    return xn_math_approx_dist2d(*(int *)(object + 7), *(int *)(object + 0xf),
                         *(int *)(player_object + 7), *(int *)(player_object + 0xf));
}

/* between the talk.c and faction.c runs: unit not certain */
iptr faction_find(short id)
{
    iptr found;
    found = faction_find_r(factions, id);
    return found;
}

/* between the talk.c and faction.c runs: unit not certain */
int faction_relation_flags(int target, int faction)
{
    scratch_190be4 = target;
    shared_scratch = 0;
    faction_relation_flags_r(faction);
    return shared_scratch;
}
