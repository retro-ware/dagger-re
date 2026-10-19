/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000410D7 */
#include "records.h"
#include "clib.h"

extern unsigned char climate_people_kind[];
extern short pedestrian_sprites[][2][4];
extern int climate_category(void);

void pedestrian_pick_sprite(struct record *pedestrian)
{
    int kind;
    int flip;

    kind = climate_category();
    flip = rand() & 1;
    if ((rand() & 31) == 0) {
        pedestrian->image = 51072;
        pedestrian->flags &= ~0x4000;
        return;
    }
    pedestrian->image = pedestrian_sprites[climate_people_kind[kind]][flip][rand() & 3] << 7;
    pedestrian->flags |= flip != 0 ? 16384 : 0;
    *((unsigned char *)pedestrian + 73) |= flip != 0 ? 16 : 0;      /* a person's data +2 flags: bit 4 female */
}
