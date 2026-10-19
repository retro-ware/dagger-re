/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000383CC */
extern unsigned char mouse_buttons;
extern char str_spells_c[];
extern unsigned char spell_effect_target_class[];
extern char *spell_effect_names[];
extern unsigned char ui_mode_flags;
extern void (*list_popup_callback)(int);
extern char *scratch_buffer;
extern int spellmaker_allowed_targets(void);
extern void spellmaker_pick_effect_cb(short);
extern int spellmaker_find_effect(short);
extern void msgbox_show_rsc(int, int);
extern void list_popup_open_strings(char *);
#include "clib.h"

int spellmaker_add_effect(void)
{
    short i;
    char *list;
    short cnt;
    char *str;
    short mode;

    if ((int)(unsigned char)(mouse_buttons & 2) != 0) {
        ui_mode_flags |= 1;
        msgbox_show_rsc(1811, 1);
        return 0;
    }
    if (spellmaker_find_effect(255) == -1) {
        msgbox_show_rsc(1707, 1);
        return 0;
    }
    mode = spellmaker_allowed_targets();
    str = scratch_buffer;
    list = scratch_buffer + 32000;
    *str = i = cnt = 0;
    for (; i < 51; i++) {
        if (spell_effect_names[i] == 0)
            continue;
        if (!((spell_effect_target_class[i] == 2 || mode == 2) || spell_effect_target_class[i] == mode))
            continue;
        list[cnt++] = i;
        mc_strncpy(str, spell_effect_names[i], 4, str_spells_c, 1108);
        str = strlen(str) + str + 1;
    }
    *str = 0;
    list_popup_callback = spellmaker_pick_effect_cb;
    list_popup_open_strings(scratch_buffer);
    return 0;
}
