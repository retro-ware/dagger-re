/* parse.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"

extern char str_parse_c[];
extern char str_fmt_d_5[];
extern signed char text_rsc_buffer[];
extern struct quest *current_quest;

extern char *text_qrc_load(char *, short, short, short);
#pragma aux mc_set_location parm routine [];

void quest_load_text(struct quest *quest, int message_id, short flags, int width)
{
    char *text;
    char file_name[16];

    current_quest = quest;
    if (quest->text_file != 0) {
        mc_set_location(2016, str_parse_c);
        mc_sprintf(file_name, str_fmt_d_5, quest->text_file);
    } else {
        mc_memcpy(file_name, quest->name, 8, str_parse_c, 2018, 13);
    }
    file_name[8] = 0;
    text = (char *)text_qrc_load(file_name, (int)(short)*(short *)&message_id, (int)(short)flags, (int)(short)*(short *)&width);
    if (text == 0) return;
    mc_strncpy((char *)text_rsc_buffer, text, 2048, str_parse_c, 2022);
    if (text == 0 || text == (char *)(iptr)-1751672937) return;
    mc_free(text, str_parse_c, 2023);
    text = (char *)(iptr)-1751672937;
}
