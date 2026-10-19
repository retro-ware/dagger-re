/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0001CF3E */
#include "records.h"
#include "clib.h"

extern char str_faction_c[];        /* __FILE__ */
extern unsigned char scratch_190d16;
extern unsigned char scratch_190d17;
extern signed char text_rsc_buffer[];
extern int shared_scratch;
extern int game_minutes;
extern unsigned char previous_region;
extern unsigned char D_001962A5;
extern int rumor_file;
extern int rumor_added_count;
extern struct faction *macro_faction2;
extern struct faction *found_faction;
extern int faction_player_related(struct faction *);
extern int func_0001D66C(struct rumor *);
extern void parse_rsc_text(int, int, int);
extern int rand_range(int, int);

void rumor_add_faction(struct faction *faction1, struct faction *faction2, int kind, unsigned char region, int text_id)
{
    int unused;
    struct rumor rumor;
    int saved;

    saved = shared_scratch;
    if (D_001962A5 == 0)
        return;
    if (text_id != 455 && text_id != 456)
        if (kind == 100 && faction_player_related(faction1) == 0 && faction_player_related(faction2) == 0) {
            shared_scratch = saved;
            return;
        }
    found_faction = faction1;
    macro_faction2 = faction2;
    scratch_190d16 = kind;
    scratch_190d17 = region;
    rumor_added_count++;
    if (region != 0)
        previous_region = region;
    else if (faction1 != 0 && faction1->region != 255)
        previous_region = faction1->region;
    else if (faction2 != 0 && faction2->region != 255)
        previous_region = faction2->region;
    else
        previous_region = rand_range(0, 61);
    parse_rsc_text(text_id, 0, 0);
    if (faction1 != 0)
        rumor.faction1 = faction1->id;
    else
        rumor.faction1 = 0;
    if (faction2 != 0)
        rumor.faction2 = faction2->id;
    else
        rumor.faction2 = 0;
    rumor.kind = kind;
    rumor.region = region;
    rumor.flags = func_0001D66C(&rumor);
    rumor.quest_id = 0;
    mc_memset(rumor.quest_name, 0, 9, str_faction_c, 1606, 9);
    rumor.message = 0;
    rumor.target = 0;
    rumor.text_length = strlen(((char *)text_rsc_buffer)) + 1;
    rumor.expires = game_minutes + 43140;
    write(rumor_file, &rumor, 34);
    write(rumor_file, ((char *)text_rsc_buffer), rumor.text_length);
    shared_scratch = saved;
}
