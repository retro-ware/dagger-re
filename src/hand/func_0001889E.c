/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0001889E */
#include "records.h"
#include "clib.h"

extern char *big_buffer;
extern char str_talk_c[];        /* __FILE__ */
extern char str_any_news[];
extern short talk_organisations[];
extern short *talk_topics;
extern int talk_list_count;
extern int talk_selected_row;
extern int talk_list_bottom;
extern int talk_list_top;
extern char talk_key_text[][65];
extern short talk_topic_count;
extern unsigned char talk_key_index;
extern void talk_list_draw_item(char *, int, int, int, int);
extern void talk_add_quest_info_topics(void);
extern struct faction *faction_find(short);

void talk_draw_tell_list(void)
{
    int i;
    int colour;
    char *name;

    talk_topics = (short *)(big_buffer + 16384);
    talk_list_count = 0;
    talk_topic_count = 1;
    talk_topics[0] = talk_topics[1] = talk_topics[2] = 0;
    if (talk_list_count >= talk_list_top && talk_list_count <= talk_list_bottom) {
        if (talk_selected_row == talk_list_count) {
            colour = 244;
            mc_strncpy(talk_key_text[talk_key_index], str_any_news, 4, str_talk_c, 1884);
        } else {
            colour = 145;
        }
        talk_list_draw_item(str_any_news, 6, (talk_list_count - talk_list_top) * 7 + 71, colour, 156);
    }
    talk_list_count++;
    talk_add_quest_info_topics();
    for (i = 0; i < 34; i++) {
        talk_topics[talk_topic_count * 3] = i > 7 ? i + 861 : i + 860;
        talk_topics[talk_topic_count * 3 + 1] = 0;
        talk_topics[talk_topic_count * 3 + 2] = 0;
        talk_topic_count++;
        if (talk_list_count < talk_list_top || talk_list_count > talk_list_bottom) {
            talk_list_count++;
            continue;
        }
        name = faction_find(talk_organisations[i])->name;
        if (talk_selected_row == talk_list_count) {
            colour = 244;
            mc_strncpy(talk_key_text[talk_key_index], name, 4, str_talk_c, 1914);
        } else {
            colour = 145;
        }
        talk_list_draw_item(name, 6, (talk_list_count - talk_list_top) * 7 + 71, colour, 156);
        talk_list_count++;
    }
}
