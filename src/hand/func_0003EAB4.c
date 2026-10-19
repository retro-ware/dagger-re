/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0003EAB4 */
#include "records.h"
#include "clib.h"

extern char str_text_c[];
extern char str_fmt_d_2[];
extern char str_qrc[];
extern short msgbox_wrap_width;
extern signed char text_buffer[];
extern signed char text_rsc_buffer[];
extern char qrc_quest_text_flag;
extern int text_rsc_file;
extern iptr msgbox_next_page;
extern struct quest *current_quest;
extern char *text_rsc_load(short, short, short);
extern void msgbox_render(char *, char **);
extern void msgbox_show_more_pages(char **);
extern int disk_open_data(char *);
#pragma aux mc_set_location parm routine [];

int msgbox_render_quest_text(struct quest *quest, short message_id, char **image, short flags)
{
    short saved_file;
    char *text;
    int single_page;

    saved_file = text_rsc_file;
    current_quest = quest;
    if (quest->text_file != 0) {
        mc_set_location(657, str_text_c);
        mc_sprintf(((char *)text_rsc_buffer), str_fmt_d_2, quest->text_file);
    } else {
        mc_memcpy(((char *)text_rsc_buffer), quest->name, 8, str_text_c, 659, 2048);
    }
    qrc_quest_text_flag = 0;
    mc_set_location(662, str_text_c);
    mc_sprintf(((char *)text_buffer), str_qrc, ((char *)text_rsc_buffer));
    if ((text_rsc_file = disk_open_data(((char *)text_buffer))) > 0) {
        text = text_rsc_load(message_id, flags | 0x8002, msgbox_wrap_width);
        if (text == 0)
            return 0;
        msgbox_render(text, image);
        if (msgbox_next_page != 0) {
            msgbox_show_more_pages(image);
            single_page = 0;
        } else {
            single_page = 1;
        }
        close(text_rsc_file);
    }
    if (text != 0 && text != (char *)(iptr)-1751672937) {
        mc_free(text, str_text_c, 685);
        text = (char *)(iptr)-1751672937;
    }
    text_rsc_file = saved_file;
    return single_page;
}
