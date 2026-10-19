/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00088E11 */
#include "records.h"
extern char *create_item_choices[];
extern signed char ui_dirty_flags;
extern int game_minutes;
extern int scratch_1a99f4;
extern void spfx_create_item_cb(int);
extern void spfx_show_choice_list(iptr, iptr);

void spfx_create_item(struct record *spell_object, int effect, int target)
{
    ui_dirty_flags &= 254;
    scratch_1a99f4 = spell_object->data.spell.cast_durations[effect] + game_minutes;
    spfx_show_choice_list((iptr)(char *)create_item_choices, (iptr)spfx_create_item_cb);
}
