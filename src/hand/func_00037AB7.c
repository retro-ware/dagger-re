/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00037AB7 */
#include "records.h"

extern struct spell *selected_spell;
extern unsigned char spell_effect_elemental[];

int spellmaker_effects_allow_element(void)
{
    short elemental;

    elemental = spell_effect_elemental[selected_spell->effects[0].type] > spell_effect_elemental[selected_spell->effects[1].type] ? spell_effect_elemental[selected_spell->effects[0].type] : spell_effect_elemental[selected_spell->effects[1].type];
    return spell_effect_elemental[selected_spell->effects[2].type] > elemental ? spell_effect_elemental[selected_spell->effects[2].type] : elemental;
}
