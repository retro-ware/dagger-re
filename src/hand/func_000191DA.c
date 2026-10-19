/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000191DA */
#include "records.h"

extern struct faction *found_faction;
extern struct faction *factions;
extern struct faction *faction_find_type_in_region_r(struct faction *, short, short);

struct faction *faction_find_type_in_region(short region, short type)
{
    struct faction *found;

    found_faction = 0;
    found = faction_find_type_in_region_r(factions, region, type);
    return found != 0 ? found : found_faction;
}
