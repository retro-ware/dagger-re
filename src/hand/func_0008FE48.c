/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0008FE48 */
#include "records.h"
#include "clib.h"

extern char str_potions_c[];
extern char str_unknown_powers[];
extern unsigned char spell_effect_settings[][12];
extern unsigned char potion_ingredient_weights[];
extern unsigned char potion_cauldron_x[];
extern unsigned char potion_cauldron_z[];
extern unsigned char potion_cauldron_rarity[];
extern short potion_cauldron_count;
extern void potion_sort_ingredients(unsigned char *, unsigned char *, unsigned char *, int);

int potion_mix_unknown(struct spell *sp)
{
    unsigned char w[8];
    unsigned char c;
    unsigned char x[8];
    int sum;
    unsigned char y[8];
    int i;
    unsigned char z[8];
    int n;

    mc_memcpy(x, potion_cauldron_x, potion_cauldron_count, str_potions_c, 410, 8);
    mc_memcpy(y, potion_cauldron_rarity, potion_cauldron_count, str_potions_c, 411, 8);
    mc_memcpy(z, potion_cauldron_z, potion_cauldron_count, str_potions_c, 412, 8);
    potion_sort_ingredients(x, y, z, potion_cauldron_count);
    n = potion_cauldron_count;
    for (i = 0; n - 1 > i; i++) {
        if (x[i] == x[i + 1]) {
            w[i] = potion_ingredient_weights[x[i]];
            y[i] += y[i + 1];
            mc_memcpy(x + i, x + (i + 1), 8 - i - 1, str_potions_c, 424, 4);
            mc_memcpy(z + i, (i + 1) + z, 8 - i - 1, str_potions_c, 425, 4);
            mc_memcpy(y + i, y + (i + 1), 8 - i - 1, str_potions_c, 426, 4);
            n--;
        }
    }
    potion_sort_ingredients(y, x, z, n);
    sum = 0;
    if (n > 3) {
        for (i = 3; i < n; i++)
            sum += y[i];
        n = 3;
    }
    switch (n) {
    case 3:
        if ((y[0] >> 1) > y[1]) {
            n = 1;
            sum += y[1] + y[2];
        } else if ((y[1] >> 1) > y[2]) {
            n = 2;
            sum += y[2];
        }
        break;
    case 2:
        if ((y[0] >> 1) > y[1]) {
            n = 1;
            sum += y[1];
        }
        break;
    }
    for (i = 0; i < n; i++) {
        if (y[i] <= sum) {
            n = i;
            break;
        }
    }
    if (n == 0) return 0;
    sp->element = 4;
    sp->target = 0;
    mc_strncpy(sp->name, str_unknown_powers, 25, str_potions_c, 482);
    sp->effects[0].type = sp->effects[1].type = sp->effects[2].type = 255;
    for (i = 0; i < n; i++) {
        c = spell_effect_settings[x[i]][z[i]];
        sp->effects[i].type = x[i];
        sp->effects[i].subtype = z[i];
        if (c & 1)
            sp->durations[i].base = sp->durations[i].plus = sp->durations[i].per_level = 1;
        if (c & 2) {
            sp->chances[i].base = 70;
            sp->chances[i].plus = sp->chances[i].per_level = 1;
        }
        if (c & 4) {
            sp->magnitudes[i].base_min = 10;
            sp->magnitudes[i].base_max = sp->magnitudes[i].plus_min = sp->magnitudes[i].base_max = sp->magnitudes[i].plus_min = 1;
        }
    }
    return 1;
}
