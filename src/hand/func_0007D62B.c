/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007D62B */
extern char mouse_buttons;
extern unsigned char text_colour;
extern unsigned char game_state_flags;
extern unsigned char msgbox_button_keys;
extern unsigned char msgbox_key_2;
extern unsigned char msgbox_hotkey;
extern unsigned char msgbox_button_ids;
extern unsigned char msgbox_button_2;
extern unsigned char msgbox_button_3;
extern char popup_answer;
extern void msgbox_show_rsc(int, int);
extern void xn_mouse_poll_clamped(void);

void msgbox_choice_rsc(short text_id, short button_1, short button_2, short button_3, unsigned char key_1, unsigned char key_2, unsigned char key_3)
{
    popup_answer = 0;
    game_state_flags |= 1;
    while (mouse_buttons != 0)
        xn_mouse_poll_clamped();
    text_colour = 146;
    msgbox_button_keys = key_1;
    msgbox_key_2 = key_2;
    msgbox_hotkey = key_3;
    msgbox_button_ids = button_1;
    msgbox_button_2 = button_2;
    msgbox_button_3 = button_3;
    msgbox_show_rsc(text_id, 5);
}
