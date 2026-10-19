/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00054B27 */
extern char mouse_buttons;
extern short mouse_x;
extern short mouse_y;
extern short font_height;
extern char **classmaker_advantage_sublists[];
extern char **classmaker_disadvantage_sublists[];
extern short classmaker_screen;
extern short classmaker_special_row;
extern short classmaker_special_level;
extern unsigned char classmaker_special_counts[];
extern short classmaker_special_list;
extern char **classmaker_picklist;
extern char mouse_buttons_prev;
extern char classmaker_specials[][7][2];
extern void classmaker_specials_picked(void);
extern void classmaker_set_advantage(int, int);
extern void classmaker_set_disadvantage(int, int);
extern void text_draw_coloured(char *, short, short, int, unsigned char);
extern void xn_mouse_poll_clamped(void);

void classmaker_specials_sublist_click(void)
{
    short top;
    short row;
    short y;
    char **names;
    short clicked;

    names = classmaker_picklist;
    y = 30;
    clicked = (mouse_buttons != 0 && mouse_buttons != mouse_buttons_prev && mouse_x > 10 && mouse_x < 161) ? 1 : 0;
    classmaker_special_row = -1;
    row = 0;
    while (*names != 0) {
        text_draw_coloured(*names, 10, y, 145, 141);
        top = y;
        y += font_height;
        if (clicked && mouse_y > top && mouse_y < y) {
            classmaker_special_row = row;
            while (mouse_buttons != 0)
                xn_mouse_poll_clamped();
        }
        row++;
        names++;
    }
    row = classmaker_special_counts[classmaker_special_list];
    classmaker_specials[classmaker_special_list][row][classmaker_special_level] = classmaker_special_row;
    if (classmaker_special_level == 0)
        classmaker_specials[classmaker_special_list][row][1] = 0;
    if (classmaker_special_row != -1 && classmaker_special_level != 0) {
        if (classmaker_special_list == 0)
            classmaker_set_advantage(row, 0);
        else
            classmaker_set_disadvantage(row, 0);
        classmaker_specials_picked();
    }
    if ((int)(short)(classmaker_screen & 16) != 0 && classmaker_special_row != -1 && (int)(short)(classmaker_screen & 15) == 2) {
        classmaker_picklist = classmaker_advantage_sublists[classmaker_special_row];
        classmaker_special_level = 1;
        if (classmaker_picklist == 0)
            classmaker_specials_picked();
    } else if ((int)(short)(classmaker_screen & 16) != 0 && classmaker_special_row != -1) {
        classmaker_picklist = classmaker_disadvantage_sublists[classmaker_special_row];
        classmaker_special_level = 1;
        if (classmaker_picklist == 0)
            classmaker_specials_picked();
    }
}
