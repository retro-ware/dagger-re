/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000411BF */
#include "records.h"
#include "clib.h"

extern char str_tfac00i0_rci[];
extern unsigned char climate_people_kind[];
extern short pedestrian_sprites[];
extern short npc_face_offsets[];
extern struct character *text_macro_npc;
extern int climate_category(void);
extern struct flat_cfg *flats_cfg_find(int);
extern int disk_open_data(char *);
extern short *xn_str_find_u16(short *, short, int);

void npc_load_face(struct record *npc, char *face)
{
    short *found_sprite;
    struct flat_cfg *flat;
    int offset;
    int file;
    short saved_seed;

    saved_seed = rand();
    srand(npc->id | (npc->id >> 16));
    found_sprite = xn_str_find_u16(pedestrian_sprites, npc->image >> 7, 24);
    if (found_sprite == 0) {
        flat = flats_cfg_find(npc->image);
        if (flat != 0 && flat->face != 0)
            offset = flat->face << 12;
        else
            offset = (npc_face_offsets[climate_people_kind[climate_category()] * 8 + (rand() & 3) + ((text_macro_npc->flags & 1) != 0 ? 4 : 0)] + rand() % 10) << 12;
    } else {
        offset = (npc_face_offsets[((iptr)found_sprite - (iptr)pedestrian_sprites) / 2] + rand() % 24) << 12;
    }
    file = disk_open_data(str_tfac00i0_rci);
    lseek(file, offset, 0);
    read(file, face, 4096);
    close(file);
    srand(saved_seed);
}
