/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0003946E */
#include "records.h"

extern struct spell *selected_spell;
extern short spell_effect_slot;
extern short spell_effect_cost_current;
extern short spell_cost_factor_1;
extern short spell_cost_factor_2;
extern short spell_cost_factor_3;

int spell_cost_duration_magnitude(void)
{
    short v;

    v = spell_effect_cost_current * selected_spell->durations[spell_effect_slot].base;
    v += spell_cost_factor_1 * (selected_spell->durations[spell_effect_slot].plus / selected_spell->durations[spell_effect_slot].per_level);
    v += spell_cost_factor_2 * ((selected_spell->magnitudes[spell_effect_slot].base_min + selected_spell->magnitudes[spell_effect_slot].base_max) / 2);
    v += ((selected_spell->magnitudes[spell_effect_slot].plus_min + selected_spell->magnitudes[spell_effect_slot].plus_max) / 2 / selected_spell->magnitudes[spell_effect_slot].per_level) * spell_cost_factor_3;
    return v;
}
