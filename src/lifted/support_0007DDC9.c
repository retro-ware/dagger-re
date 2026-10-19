/* support.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */

extern signed char text_colour;
extern signed char msgbox_button_keys;
extern signed char msgbox_key_2;
extern signed char msgbox_button_ids;
extern signed char msgbox_button_2;
extern signed char msgbox_button_3;
extern unsigned char popup_answer;

extern void msgbox_show_rsc(int, int);

void msgbox_yes_no_rsc(int text_id)
{
    popup_answer = 0;
    msgbox_button_ids = 4;
    msgbox_button_2 = 5;
    msgbox_button_3 = 0;
    msgbox_button_keys = 21;
    msgbox_key_2 = 49;
    text_colour = 146;
    msgbox_show_rsc((int)(short)text_id, 5);
}
