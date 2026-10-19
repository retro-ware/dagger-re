/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00014AE8 */
#include "records.h"
#include "clib.h"

extern char str_talk01i0_img[];
extern char str_talk02i0_img[];
extern char str_talk03i0_img[];
extern char str_talk_c[];
extern char world_running;
extern char talk_wrap_to_question;
extern char talk_face_loaded;
extern iptr window_image;
extern char *scratch_buffer;
extern struct talk_where *talk_where_target;
extern int talk_disposition;
extern unsigned char game_mode_under_popup;
extern unsigned char D_00196272;
extern unsigned char game_mode;
extern unsigned char crime_current;
extern unsigned char people_witness_flags;
extern void *talk_saved_screen;
extern struct faction *talk_npc_own_faction;
extern struct faction *talk_npc_faction;
extern struct talk_place_topic *talk_place_topics;
extern int talk_npc_speech_style;
extern int talk_npc_attitude;
extern int talk_rand_seed;
extern int talk_attitude_cache;
extern int talk_attitude_cache_2;
extern int talk_etiquette_counted;
extern struct record *talk_npc_object;
extern iptr talk_tabs_image;
extern int talk_selected_row;
extern iptr talk_face_image;
extern int talk_list_bottom;
extern int talk_list_top;
extern iptr talk_mode_image;
extern struct talk_where talk_where;
extern short talk_okay_state;
extern unsigned char talk_flags;
extern short talk_faction_disposition;
extern char talk_question_mode;
extern char talk_showing_categories;
extern char talk_topic_tab;
extern char talk_tone;
extern char talk_redraw;
extern char talk_news_asked;
extern char talk_question_ready;
extern char talk_prostitute_offer;
extern char talk_refused;
extern char talk_logbook_ready;
extern void talk_close(void);
extern void talk_say_text(int);
extern void talk_say_string(iptr);
extern void talk_init_text(void);
extern void talk_build_place_topics(void);
extern void func_00018339(void);
extern int talk_faction_greeting(int);
extern int talk_roll_attitude(void);
extern iptr func_0001D46A(int);
extern void msgbox_show_rsc(short, int);
extern void guards_summon(int);
extern void npc_load_face(struct record *, iptr);
extern int people_check_witnesses(void);
extern iptr disk_read_file(char *, iptr);
extern void xn_font_select(int);

int talk_open(struct record *npc)
{
    iptr rumor_text;
    int greeting;

    if (game_mode_under_popup == 12 && game_mode == 8)
        return 1;
    if (npc != 0) {
        if (talk_npc_faction->social_group == 4 && (people_check_witnesses() & 2)) {
            people_witness_flags &= 2;
            crime_current = 7;
            guards_summon(0);
            return 0;
        }
        talk_news_asked = 0;
        talk_refused = 0;
        talk_prostitute_offer = 0;
        talk_npc_object = npc;
        xn_font_select(4);
        game_mode = 12;
        window_image = disk_read_file(str_talk01i0_img, 0);
        talk_tabs_image = disk_read_file(str_talk02i0_img, 0);
        talk_mode_image = disk_read_file(str_talk03i0_img, 0);
        talk_saved_screen = mc_malloc(64000, str_talk_c, 225);
        D_00196272 = 1;
        talk_logbook_ready = 0;
        talk_tone = 1;
        talk_question_mode = 1;
        talk_topic_tab = 0;
        talk_list_top = 0;
        talk_list_bottom = 13;
        talk_selected_row = 0;
        talk_redraw = 1;
        talk_okay_state = 0;
        talk_question_ready = 0;
        talk_faction_disposition = 0;
        talk_attitude_cache = talk_attitude_cache_2 = talk_etiquette_counted = 0;
        talk_where_target = &talk_where;
        talk_place_topics = (struct talk_place_topic *)(scratch_buffer + 64768);
        func_00018339();
        talk_showing_categories = 1;
        talk_build_place_topics();
        talk_flags |= 2;
        world_running = 0;
        talk_wrap_to_question = 0;
        talk_init_text();
        talk_rand_seed = rand();
        if (talk_face_loaded == 0)
            npc_load_face(talk_npc_object, talk_face_image);
        rumor_text = func_0001D46A(talk_npc_object->id);
        if (talk_npc_own_faction->type == 15 || talk_npc_own_faction->type == 14)
            greeting = talk_npc_own_faction->id;
        else
            greeting = talk_npc_faction->id;
        if (talk_npc_own_faction->type == 15 || talk_npc_own_faction->type == 14) {
            if (talk_npc_own_faction->social_group < 5)
                talk_npc_speech_style = talk_npc_own_faction->social_group;
            else
                talk_npc_speech_style = 1;
        } else {
            if (talk_npc_faction->social_group < 5)
                talk_npc_speech_style = talk_npc_faction->social_group;
            else
                talk_npc_speech_style = 1;
        }
        if (talk_npc_speech_style == 1)
            talk_npc_speech_style = 0;
        else if (talk_npc_speech_style == 0)
            talk_npc_speech_style = 1;
        talk_npc_attitude = talk_roll_attitude();
        if (rumor_text != 0) {
            talk_say_string(rumor_text);
        } else {
            greeting = talk_faction_greeting(greeting);
            if (greeting != 0) {
                if (talk_refused) {
                    talk_close();
                    msgbox_show_rsc(greeting, 1);
                    return 0;
                }
                talk_say_text(greeting);
            } else if (talk_disposition < 0) {
                talk_say_text(7206);
            } else if (talk_disposition < 10) {
                talk_say_text(7207);
            } else if (talk_disposition >= 10 && talk_disposition < 30) {
                talk_say_text(7208);
            } else {
                talk_say_text(7209);
            }
        }
    }
    return game_mode == 12 ? 1 : 0;
}
