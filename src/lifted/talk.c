/* talk.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "bitfield.h"
#include "clib.h"

extern signed char mouse_buttons;
extern signed char mouse_double_click;
extern short mouse_x;
extern short mouse_y;
extern signed char text_colour;
extern signed char key_down_esc;
extern signed char key_down_enter;
extern signed char key_down_up;
extern signed char key_down_pgup;
extern signed char key_down_down;
extern signed char key_down_pgdn;
extern short xn_gfx_clip_left;
extern short xn_gfx_clip_right;
extern iptr screen_buffer;
extern iptr big_buffer;
extern char str_talk_c[];
extern char str_no_response[];
extern char str_empty[];
extern char str_previous_list[];
extern char str_any[];
extern signed char char_class_table[];
extern unsigned char player_environment;
extern char talk_faction_greetings[];
extern char str_g_h_i_j_k_l_m_n_o_p_q_r_r_s_t_t_u_v_v_w[];
extern char str_h_i_j_k_l_m_n_o_p_q_r_r_s_t_t_u_v_v_w_x[];
extern short talk_question_attitude_mods[];
extern char talk_knows_mods[];
extern char talk_where_answers[];
extern char talk_tell_answers[];
extern char talk_creature_language[];
extern char talk_race_language_texts[];
extern struct rect talk_buttons[19];
extern char talk_greeting_texts[];
extern short talk_ack_texts[];
extern short talk_regional_ids[];
extern short talk_regional_services[];
extern signed char talk_regional_regions[];
extern char talk_category_building_types[];
extern iptr talk_category_names[];
extern iptr talk_regional_names[];
extern char region_neighbours[];
extern signed char world_running;
extern struct region regions[];
extern signed char text_buffer[];
extern char scratch_190be4[];
extern signed char talk_wrap_to_question;
extern signed char talk_face_loaded;
extern signed char talk_hint_quest_id;
extern signed char text_rsc_buffer[];
extern char talk_garbled_text[];
extern signed char game_state_flags;
extern char text_macro_fcn[];
extern struct record *quest_root;
extern struct character *text_macro_npc;
extern struct record *player_object;
extern char shared_scratch[];
extern struct location *current_location;
extern struct character *player_character;
extern iptr window_image;
extern struct settings *game_settings;
extern struct record *quest_tick_object;
extern struct talk_where *talk_where_target;
extern int talk_disposition;
extern int talk_prostitute_price;
extern short text_macro_n_text;
extern signed char current_region;
extern unsigned char popup_answer;
extern signed char screen_active;
extern signed char game_mode;
extern signed char mouse_buttons_prev;
extern char talk_regional_rows[];
extern signed char talk_place_rows[];
extern char *talk_question_lines;
extern int talk_place_topic_count;
extern int talk_list_max_width;
extern short *talk_topics;
extern char talk_list_scroll_x[];
extern iptr talk_text_pool_next;
extern char *talk_answer_lines;
extern iptr talk_saved_screen;
extern struct faction *talk_npc_own_faction;
extern struct faction *talk_npc_faction;
extern struct talk_place_topic *talk_place_topics;
extern struct character *talk_npc_record;
extern iptr talk_text_pool;
extern int talk_npc_speech_style;
extern int talk_answer_line_count;
extern iptr talk_question_line_buffer;
extern int talk_answer_scroll;
extern int talk_list_count;
extern int talk_npc_attitude;
extern int talk_npc_knows;
extern iptr talk_answer_line_buffer;
extern int talk_rand_seed;
extern int talk_attitude_cache[];
extern int talk_etiquette_counted;
extern int talk_question_line_count;
extern int talk_answer_kind;
extern struct record *talk_npc_object;
extern iptr talk_tabs_image;
extern char talk_selected_row[];
extern iptr talk_face_image;
extern int talk_list_bottom;
extern int talk_list_top;
extern iptr talk_mode_image;
extern signed char talk_category_rows[];
extern char talk_key_text[];
extern short talk_topic_count;
extern short talk_okay_state;
extern short talk_language_text;
extern short talk_language_skill;
extern char talk_flags[];
extern short talk_faction_disposition;
extern signed char talk_question_mode;
extern signed char talk_showing_categories;
extern signed char talk_topic_tab;
extern signed char talk_tone;
extern signed char talk_redraw;
extern signed char talk_news_asked;
extern signed char talk_location_category;
extern unsigned char talk_prostitute_state;
extern signed char talk_question_ready;
extern signed char talk_key_index;
extern unsigned char talk_prostitute_offer;
extern signed char talk_refused;
extern signed char talk_logbook_ready;
extern struct faction *macro_faction2;
extern int faction_count;
extern struct faction *found_faction;
extern struct faction *factions;
extern struct quest *current_quest;
extern struct quest *quest_tick_data;

extern int talk_open(struct record *);
extern int talk_hint_text_id(int);
extern int town_has_building(short, int);
extern int building_distance(struct building *);
extern int talk_faction_relation(short);
extern struct faction *faction_find(short);
extern iptr rumor_pick_news(short);
extern iptr quest_symbol_text(int, int, int);
extern iptr quest_section(iptr, int);
extern struct record *quest_find_site_for_building(struct building *);
extern iptr parse_bio_answer_text(int);
extern iptr quest_find_by_id(int);
extern iptr quest_find_potential_questor(void);
extern struct character *npc_talk_record_build(struct record *);
extern int font_char_width(unsigned char);
extern int font_text_width(iptr);
extern int player_reaction_mod(int);
extern int item_artifact_equipped(int);
extern int sound_play(int, struct record *, int);
extern struct membership *guild_find_membership_by_bits(unsigned char);
extern int guild_local_temple_rank(void);
extern iptr hud_message_add(char *);
extern int key_pressed_once(unsigned char);
extern int rand_range(int, int);
extern struct building *object_building(struct record *);
extern iptr npc_display_name(struct record *);
extern iptr building_name(iptr);
extern int xn_mouse_poll_clamped(void);
extern void xn_draw_fill_rect(int, int, int, int);
extern void xn_draw_image(int, int, int, int, char *);
extern void talk_prostitute_service(void);
extern void talk_load_face(int);
extern void talk_draw_answer(int, int, int, int);
extern void talk_draw_question(int, int, int, int);
extern void talk_draw_tell_list(void);
extern void skill_add_uses(int, int);
extern void msgbox_show_rsc(int, int);
extern void parse_rsc_text(int, int, int);
extern void quest_load_text(struct quest *, int, short, int);
extern void time_pass(int);
extern void logbook_copy_text(iptr);
extern void text_draw_coloured(char *, int, int, int, int);
extern void text_draw_centred_coloured(iptr, int, int, int, unsigned char);
extern void msgbox_yes_no_rsc(int);
extern void location_free(struct loaded_location *);
extern void location_pick_random_with_service(struct loaded_location *, int, int);
int talk_ask_news(void);
int talk_okay_hook(void);
int talk_prostitute_available(void);
iptr talk_garble_word(iptr);
int talk_next_word(signed char **);
iptr talk_alloc_line(void);
iptr building_category(struct building *);
int town_has_category(iptr);
int talk_roll_knows(void);
int talk_roll_attitude(void);
struct faction *faction_nth_r(struct faction *);
struct faction *faction_nth_of_type_r(struct faction *, unsigned char);
int faction_is_regional_noble(struct faction *);
int faction_has_enemy(struct faction *, struct faction *);
int faction_has_ally(struct faction *, struct faction *);
int faction_regions_border(struct faction *, struct faction *);
void talk_draw(void);
void talk_prepare_where_answer(void);
void talk_close(void);
void talk_list_scroll_up(void);
void talk_list_scroll_down(void);
void talk_say_text(int);
void talk_draw_face(void);
void talk_wrap_text(signed char *, int, int, int, int);
void talk_log_question(void);
void talk_add_line(iptr);
void talk_clear_question(void);
void talk_draw_place_list(void);
void talk_list_click(int);
void talk_add_quest_topics(int);
void talk_list_draw_item(iptr, int, int, int, int);
void talk_draw_regional_list(void);
void talk_add_regional_item(int);
void talk_draw_categories(void);
void faction_count_of_type_r(struct faction *, unsigned char);
void faction_max_power_r(struct faction *);
void faction_add_reputation(struct faction *, int);
void faction_add_reputation_r(struct faction *, struct faction *, int);
void faction_propagate_reputation(struct faction *, int);
#pragma aux mc_set_location parm routine [];

void talk_update(void)
{
    int button;

    if (talk_open(0) == 0) return;
    if (talk_redraw == 0) {
        mc_memcpy((void *)screen_buffer, (void *)talk_saved_screen, 64000, str_talk_c, 324, 4);
    } else {
        mc_memcpy((void *)screen_buffer, (void *)window_image, 64000, str_talk_c, 326, 4);
    }
    if (talk_redraw != 0) talk_draw();
    if (key_down_up != 0) {
        talk_list_scroll_up();
    } else if (key_down_down != 0) {
        talk_list_scroll_down();
    } else if (key_down_pgup != 0) {
        for (button = 0; button < 10; button++) {
            talk_list_scroll_up();
        }
    } else if (key_down_pgdn != 0) {
        for (button = 0; button < 10; button++) {
            talk_list_scroll_down();
        }
    }
    if (((talk_topic_tab == 0 && talk_showing_categories != 0) || talk_topic_tab != 0) && key_down_esc != 0) {
        talk_close();
    }
    if (key_down_esc != 0) {
        talk_redraw = 1;
        while (key_down_esc != 0);
        talk_showing_categories = 1;
    }
    if (((struct bf8_2_1 *)&game_state_flags)->f != 0 || ((mouse_buttons == 0 || (mouse_buttons != 0 && mouse_buttons_prev != 0)) && (key_down_enter == 0 && key_down_esc == 0))) {
        return;
    }
    if (talk_refused != 0) {
        for (button = 10; button < 17; button++) {
            if (mouse_x > talk_buttons[button].x0 && mouse_x < talk_buttons[button].x1 && mouse_y > talk_buttons[button].y0 && mouse_y < talk_buttons[button].y1) {
                sound_play(203, player_object, 110);
                talk_buttons[button].handler(button);
            }
        }
        return;
    }
    for (button = 0; button < 19; button++) {
        if (mouse_x > talk_buttons[button].x0 && mouse_x < talk_buttons[button].x1 && mouse_y > talk_buttons[button].y0 && mouse_y < talk_buttons[button].y1) {
            if (mouse_buttons != 0 || button == 15) {
                sound_play(203, player_object, 110);
                talk_buttons[button].handler(button);
            }
        }
    }
}

void talk_draw(void)
{
    int src_offset;
    int unused1;
    int offset;
    int seed;
    int unused2;

    seed = rand();
    srand(talk_rand_seed);
    if (talk_npc_own_faction->type == 4) {
        text_draw_centred_coloured((iptr)((char *)talk_npc_own_faction + 3), 213, 53, 145, 156);
    } else {
        text_draw_centred_coloured(npc_display_name(talk_npc_object), 213, 53, 145, 156);
    }
    talk_draw_face();
    talk_draw_answer(190, 66, 302, 184);
    talk_clear_question();
    talk_list_max_width = 0;
    text_colour = 244;
    xn_draw_fill_rect(talk_buttons[((int)(unsigned char)talk_tone) + 6].x0, talk_buttons[((int)(unsigned char)talk_tone) + 6].y0, (int)(short)((talk_buttons[((int)(unsigned char)talk_tone) + 6].x1 - talk_buttons[((int)(unsigned char)talk_tone) + 6].x0) + 1), (int)(short)((talk_buttons[((int)(unsigned char)talk_tone) + 6].y1 - talk_buttons[((int)(unsigned char)talk_tone) + 6].y0) + 1));
    xn_draw_image(4, 26, 107, 40, (char *)talk_tabs_image);
    if (talk_question_mode != 0) {
        for (offset = (talk_buttons[((int)(unsigned char)talk_topic_tab) + 2].y0) * 320; ((talk_buttons[((int)(unsigned char)talk_topic_tab) + 2].y1) * 320) >= offset; offset += 320) {
            mc_memcpy((void *)((talk_buttons[((int)(unsigned char)talk_topic_tab) + 2].x0) + (screen_buffer + offset)), (void *)((window_image + offset) + (talk_buttons[((int)(unsigned char)talk_topic_tab) + 2].x0)), 107, str_talk_c, 417, 4);
        }
    }
    src_offset = ((int)(unsigned char)talk_question_mode) * 1070;
    for (offset = (talk_buttons[(int)(unsigned char)talk_question_mode].y0) * 320; ((talk_buttons[(int)(unsigned char)talk_question_mode].y1) * 320) >= offset; offset += 320, src_offset += 107) {
        mc_memcpy((void *)((talk_buttons[(int)(unsigned char)talk_question_mode].x0) + (screen_buffer + offset)), (void *)(talk_mode_image + src_offset), 107, str_talk_c, 423, 4);
    }
    talk_question_ready = 1;
    if (((int)(unsigned char)talk_topic_tab) == 3) {
        parse_rsc_text(7211, 0, 0);
        mc_strncpy(talk_key_text, (char *)text_rsc_buffer, 4, str_talk_c, 430);
        parse_rsc_text(((int)(unsigned char)talk_tone) + 7212, 0, 0);
        talk_wrap_to_question = 1;
        mc_strncpy((((char *)big_buffer) + 90000), (char *)text_rsc_buffer, 4, str_talk_c, 433);
        talk_wrap_text((signed char *)(((char *)big_buffer) + 90000), 134, 10, 245, 45);
        talk_wrap_to_question = 0;
    } else {
        switch ((unsigned char)talk_question_mode) {
        case 1:
            if (talk_showing_categories != 0 && talk_topic_tab == 0) {
                talk_draw_categories();
            } else {
                talk_draw_place_list();
                if (talk_list_count == 0) break;
                parse_rsc_text(((int)(unsigned char)talk_tone) + 7225, 0, 0);
                talk_wrap_to_question = 1;
                mc_strncpy((((char *)big_buffer) + 90000), (char *)text_rsc_buffer, 4, str_talk_c, 448);
                talk_wrap_text((signed char *)(((char *)big_buffer) + 90000), 134, 10, 245, 45);
                talk_wrap_to_question = 0;
            }
            break;
        case 0:
            talk_draw_tell_list();
            if (*(short *)((((char *)talk_topics) + (*(int *)talk_selected_row * 6))) == 0 && talk_topics[*(int *)talk_selected_row * 3 + 1] == 0) {
                parse_rsc_text(((int)(unsigned char)talk_tone) + 7231, 0, 0);
            } else {
                parse_rsc_text(((int)(unsigned char)talk_tone) + 7212, 0, 0);
            }
            talk_wrap_to_question = 1;
            mc_strncpy((((char *)big_buffer) + 90000), (char *)text_rsc_buffer, 4, str_talk_c, 461);
            talk_wrap_text((signed char *)(((char *)big_buffer) + 90000), 134, 10, 245, 45);
            talk_wrap_to_question = 0;
        }
    }
    talk_draw_question(125, 10, 245, 45);
    mc_memcpy((void *)talk_saved_screen, (void *)screen_buffer, 64000, str_talk_c, 469, 4);
    talk_redraw = 0;
    srand(seed);
}

void talk_start(struct record *npc)
{
    struct faction *faction;

    talk_face_image = 0;
    talk_face_loaded = 0;
    talk_npc_object = npc;
    if (((int)(unsigned short)(npc->npc_flags & 32768)) != 0) {
        msgbox_show_rsc(7205, 1);
        return;
    }
    if (npc->type != 8 && npc->type != 18 && npc->type != 53) {
        msgbox_show_rsc(7204, 1);
        return;
    }
    if (npc->type == 53 && ((int)(unsigned short)(npc->image2 & 1)) != 0) {
        hud_message_add(str_no_response);
        return;
    }
    *(short *)talk_flags = 0;
    if (npc->type == 8 || npc->type == 53) {
        talk_npc_record = npc_talk_record_build(npc);
        *(signed char *)talk_flags &= 254;
    } else {
        if ((talk_npc_record = &npc->data.character)->race < 43) *(signed char *)talk_flags |= 1;
        talk_language_skill = *(short *)(talk_creature_language + (talk_npc_record->race << 2));
        talk_language_text = *(short *)(talk_race_language_texts + (talk_npc_record->race << 2));
        if (talk_language_skill == 0) {
            msgbox_show_rsc(7204, 1);
            return;
        }
        if (((int)(unsigned short)talk_language_skill) == 1) {
            *(signed char *)talk_flags &= 254;
        }
    }
    text_macro_npc = talk_npc_record;
    talk_npc_faction = faction_find(talk_npc_record->faction_id);
    talk_face_image = (iptr)mc_malloc(4096, str_talk_c, 530);
    if (talk_npc_faction->type == 4) talk_load_face(talk_npc_faction->face);
    faction = talk_npc_faction;
    while (faction != 0) {
        if (faction->type == 2 || faction->type == 7 || faction->type == 9) break;
        faction = faction->parent;
    }
    talk_npc_own_faction = talk_npc_faction;
    if (faction != 0) talk_npc_faction = faction;
    talk_disposition = talk_npc_faction->reputation;
    talk_disposition += guild_local_temple_rank();
    if (talk_npc_faction->social_group < 5) {
        talk_disposition += player_character->reputation[talk_npc_faction->social_group];
        talk_disposition += player_reaction_mod(talk_npc_faction->social_group);
    } else {
        talk_disposition += player_reaction_mod(100);
    }
    if (item_artifact_equipped(0) != 0) {
        talk_disposition += player_character->attributes[5] / 5;
    }
    if (talk_disposition < (-20)) {
        msgbox_show_rsc(7205, 1);
        if (talk_face_image != 0 && talk_face_image != (-1751672937)) {
            mc_free((void *)talk_face_image, str_talk_c, 562);
            talk_face_image = -1751672937;
        }
        return;
    }
    talk_prostitute_offer = 0;
    if (talk_npc_record->faction_id == 512) {
        if ((((int)(unsigned short)((short)player_character->flags & 1)) ^ ((int)(unsigned short)(talk_npc_record->flags & 1))) != 0 && ((int)(unsigned short)(game_settings->view_flags & 4)) == 0 && talk_prostitute_available() != 0) {
            talk_prostitute_offer = 1;
            talk_prostitute_state = 0;
            return;
        }
    }
    talk_open(talk_npc_object);
}

int talk_prostitute_update(void)
{
    switch (talk_prostitute_offer) {
    case 1:
        switch (talk_prostitute_state) {
        case 0:
            msgbox_yes_no_rsc(7200);
            talk_prostitute_state = 1;
            break;
        case 1:
            if (((int)popup_answer) == 2) {
                talk_open(talk_npc_object);
            } else if ((talk_prostitute_price = rand_range(50, 75) - talk_disposition) > 0) {
                msgbox_yes_no_rsc(7202);
                talk_prostitute_state = 3;
            } else {
                msgbox_yes_no_rsc(7201);
                talk_prostitute_state = 2;
            }
            break;
        case 3:
            if (((int)popup_answer) != 1) goto L15B7E;
            player_character->gold -= talk_prostitute_price;
            if (((unsigned)(player_character->gold + talk_prostitute_price)) >= talk_prostitute_price) goto L15B7E;
            player_character->gold += talk_prostitute_price;
            msgbox_show_rsc(7203, 1);
            talk_prostitute_offer = 0;
            break;
        case 2:
L15B7E:;
            if (((int)popup_answer) == 2) {
                talk_open(talk_npc_object);
            } else {
                talk_prostitute_service();
                time_pass(player_character->attributes[4]);
                talk_open(talk_npc_object);
            }
        }
    }
    return (int)talk_prostitute_offer;
}

void talk_button_where_is(void)
{
    talk_list_top = 0;
    talk_list_bottom = 13;
    *(int *)talk_list_scroll_x = 0;
    *(int *)talk_selected_row = 1;
    talk_question_mode = 1;
    talk_redraw = 1;
}

void talk_prepare_where_answer(void)
{
    talk_npc_knows = talk_roll_knows();
    talk_answer_kind = 2;
    if ((unsigned char)talk_topic_tab > 0) {
        talk_where_target->building = talk_place_topics[*(int *)talk_selected_row].building;
        talk_where_target->pad04 = 0;
        return;
    }
    if (((int)(unsigned char)talk_location_category) == 14) {
        talk_where_target->kind = 5;
        talk_where_target->region = current_region;
        talk_where_target->building = 0;
        talk_where_target->pad04 = 0;
        return;
    }
    talk_where_target->kind = 5;
    talk_where_target->region = current_region;
    talk_where_target->building = talk_place_topics[(unsigned char)talk_place_rows[*(int *)talk_selected_row]].building;
    talk_where_target->pad04 = 0;
}

void talk_button_tell_me_about(void)
{
    talk_list_top = 0;
    talk_list_bottom = 13;
    *(int *)talk_list_scroll_x = 0;
    *(int *)talk_selected_row = 0;
    talk_question_mode = 0;
    talk_redraw = 1;
}

int talk_ask_news(void)
{
    iptr news;

    talk_npc_knows = talk_roll_knows();
    talk_answer_kind = 3;
    if (*(int *)talk_selected_row != 0) return 0;
    news = rumor_pick_news((int)(short)talk_npc_faction->id);
    if (news == 0) return 0;
    mc_strncpy((char *)(big_buffer + 90000), (char *)news, 4, str_talk_c, 699);
    talk_logbook_ready = 1;
    mc_strncpy((char *)(big_buffer + 95000), (char *)news, 4, str_talk_c, 701);
    talk_wrap_text((signed char *)(big_buffer + 90000), 190, 66, 302, 191);
    return 1;
}

void talk_tab_location(void)
{
    *(int *)talk_selected_row = 1;
    *(int *)talk_list_scroll_x = 0;
    talk_topic_tab = 0;
    talk_redraw = 1;
}

void talk_tab_people(void)
{
    *(int *)talk_list_scroll_x = 0;
    talk_topic_tab = 1;
    talk_redraw = 1;
}

void talk_tab_things(void)
{
    *(int *)talk_list_scroll_x = 0;
    talk_topic_tab = 2;
    talk_redraw = 1;
}

void talk_tab_work(void)
{
    *(int *)talk_list_scroll_x = 0;
    talk_topic_tab = 3;
    talk_redraw = 1;
}

void talk_button_tone(int button)
{
    talk_tone = *(signed char *)&button - 6;
    talk_npc_attitude = talk_roll_attitude();
    talk_redraw = 1;
}

void talk_button_copy_to_logbook(void)
{
    if (talk_logbook_ready == 0) return;
    logbook_copy_text(big_buffer + 95000);
}

void talk_close(void)
{
    int unused;

    while (key_down_esc != 0);
    while (mouse_buttons != 0) xn_mouse_poll_clamped();
    if (talk_refused != 0) {
        talk_npc_object->npc_flags |= 0x8000;
        if (talk_npc_object->type == 53) talk_npc_object->image2 |= 1;
    }
    if (talk_face_image != 0 && talk_face_image != (-1751672937)) {
        mc_free((void *)talk_face_image, str_talk_c, 760);
        talk_face_image = -1751672937;
    }
    talk_face_loaded = 0;
    if (window_image != 0 && window_image != (-1751672937)) {
        mc_free((void *)window_image, str_talk_c, 762);
        window_image = -1751672937;
    }
    if (talk_tabs_image != 0 && talk_tabs_image != (-1751672937)) {
        mc_free((void *)talk_tabs_image, str_talk_c, 763);
        talk_tabs_image = -1751672937;
    }
    if (talk_saved_screen != 0 && talk_saved_screen != (-1751672937)) {
        mc_free((void *)talk_saved_screen, str_talk_c, 764);
        talk_saved_screen = -1751672937;
    }
    if (talk_mode_image != 0 && talk_mode_image != (-1751672937)) {
        mc_free((void *)talk_mode_image, str_talk_c, 765);
        talk_mode_image = -1751672937;
    }
    game_mode = 0;
    screen_active = 0;
    world_running = 1;
}

void talk_list_scroll_up(void)
{
    if (talk_topic_tab == 0 && ((int)(unsigned char)talk_question_mode) == 1 && talk_showing_categories != 0) {
        return;
    }
    if (talk_list_top == 0) return;
    talk_list_top--;
    talk_list_bottom = talk_list_top + 13;
    talk_redraw = 1;
}

void talk_list_scroll_down(void)
{
    if (talk_topic_tab == 0 && ((int)(unsigned char)talk_question_mode) == 1 && talk_showing_categories != 0) {
        return;
    }
    if (talk_list_bottom >= talk_list_count) return;
    talk_list_top++;
    talk_list_bottom = talk_list_top + 13;
    talk_redraw = 1;
}

void talk_list_scroll_left(void)
{
    if (*(int *)talk_list_scroll_x >= 0) return;
    *(int *)talk_list_scroll_x += 4;
    talk_redraw = 1;
}

void talk_list_scroll_right(void)
{
    if (((talk_list_max_width + 6) + *(int *)talk_list_scroll_x) <= 100) return;
    *(int *)talk_list_scroll_x -= 4;
    talk_redraw = 1;
}

void talk_answer_scroll_up(void)
{
    if (talk_answer_scroll != 0) (talk_answer_scroll)--;
    talk_redraw = 1;
}

void talk_answer_scroll_down(void)
{
    if ((talk_answer_line_count - 18) > talk_answer_scroll) {
        talk_answer_scroll++;
    }
    talk_redraw = 1;
}

void talk_button_okay(void)
{
    if (talk_okay_hook() != 0) return;
    if (talk_question_mode != 0 && talk_topic_tab == 0 && talk_showing_categories != 0) {
        talk_list_click(100);
        return;
    }
    if ((talk_question_mode != 0 && talk_topic_tab == 0 && talk_showing_categories != 0) || talk_question_ready == 0) {
        return;
    }
    talk_rand_seed = rand();
    talk_redraw = 1;
    talk_okay_state = 0;
    talk_log_question();
    switch ((unsigned char)talk_question_mode) {
    case 1:
        talk_prepare_where_answer();
        break;
    case 0:
        if (talk_news_asked != 0) {
            talk_say_text(1457);
            *(signed char *)talk_flags &= 253;
            return;
        }
        if (talk_npc_own_faction->id != 806 && talk_npc_own_faction->id != 842) {
            talk_news_asked = 1;
        }
        if (talk_ask_news() != 0) {
            *(signed char *)talk_flags &= 253;
            if (*(int *)talk_selected_row == 0) return;
        }
    }
    *(signed char *)talk_flags &= 253;
    if (((int)(unsigned char)talk_topic_tab) == 3) {
        if (((int)player_environment) == 3 || quest_find_potential_questor() == 0) {
            talk_say_text(8078);
            return;
        }
        if (talk_npc_attitude == 0) {
            talk_say_text(8075);
        } else if (talk_npc_attitude == 1) {
            talk_say_text(8076);
        } else {
            talk_say_text(8077);
        }
        return;
    }
    if (talk_question_mode == 0) {
        talk_say_text((int)(short)*(short *)(talk_tell_answers + (((talk_npc_knows * 30) + (talk_npc_speech_style * 6)) + (talk_npc_attitude * 2))));
        return;
    }
    if (((int)(unsigned char)talk_question_mode) == 1 && ((int)(unsigned char)talk_location_category) == 14) {
        talk_say_text(28);
        return;
    }
    talk_say_text((int)(short)*(short *)(talk_where_answers + (((talk_npc_knows * 30) + (talk_npc_speech_style * 6)) + (talk_npc_attitude * 2))));
}

void talk_say_indexed(iptr text_ids)
{
    talk_say_text(*(int *)((char *)((talk_answer_kind << 2) + text_ids)));
}

int talk_okay_hook(void)
{
    return 0;
}

int talk_prostitute_available(void)
{
    return 1;
}

void talk_say_text(int text_id)
{
    parse_rsc_text(text_id, 0, 0);
    mc_strncpy((char *)(big_buffer + 90000), (char *)text_rsc_buffer, 4, str_talk_c, 934);
    talk_logbook_ready = 1;
    mc_strncpy((char *)(big_buffer + 95000), (char *)text_rsc_buffer, 4, str_talk_c, 936);
    talk_wrap_text((signed char *)(big_buffer + 90000), 190, 66, 302, 191);
}

void talk_say_string(iptr text)
{
    mc_strncpy((char *)(big_buffer + 90000), (char *)text, 4, str_talk_c, 942);
    talk_logbook_ready = 1;
    mc_strncpy((char *)(big_buffer + 95000), (char *)text, 4, str_talk_c, 944);
    talk_wrap_text((signed char *)(big_buffer + 90000), 190, 66, 302, 191);
}

iptr talk_macro_hint(int variant)
{
    int text_id;

    text_id = talk_hint_text_id(variant);
    if ((text_id & 32768) != 0) {
        *(iptr *)&current_quest = (*(iptr *)&quest_tick_data = quest_find_by_id((int)(short)(short)talk_hint_quest_id));
        quest_load_text(current_quest, text_id & 32767, 0, 0);
    } else {
        parse_rsc_text(text_id, 0, 0);
    }
    return (iptr)text_rsc_buffer;
}

void talk_translate(signed char *src, signed char *dest)
{
    int length;
    int in_word;

    length = 0;
    in_word = 0;
    while (*src != 0) {
        if (((int)(unsigned char)(char_class_table[(int)(unsigned char)(*src + 1)] & 192)) != 0) {
            text_buffer[length++] = *src;
            in_word = 1;
        } else {
            if (in_word != 0) {
                text_buffer[length] = 0;
                in_word = 0;
                length = in_word;
                mc_strncpy(dest, (char *)talk_garble_word((iptr)text_buffer), 4, str_talk_c, 1038);
                dest += strlen(dest);
            }
            *dest++ = *src++;
        }
    }
}

iptr talk_garble_word(iptr word)
{
    int count;
    int i;

    player_character->skills[(int)(unsigned short)talk_language_skill].uses = 1;
    if (rand_range(1, 100) <= player_character->skills[(int)(unsigned short)talk_language_skill].value) {
        return word;
    }
    count = rand_range(2, 4);
    *(signed char *)talk_garbled_text = 0;
    for (i = 0; i < count; i++) {
        func_000A1054(talk_garbled_text, (char *)parse_bio_answer_text((int)(unsigned short)talk_language_text), str_talk_c, 1059, 4);
    }
    return (iptr)talk_garbled_text;
}

void talk_draw_face(void)
{
    xn_draw_image(119, 65, 64, 64, (char *)talk_face_image);
}

void talk_wrap_text(signed char *text, int left, int top, int right, int bottom)
{
    int word_width;
    int line_width;

    line_width = 0;
    text_rsc_buffer[0] = 0;
    if (talk_wrap_to_question == 0 && talk_answer_line_count != 0) {
        talk_add_line((iptr)str_empty);
    }
    while (*text != 0) {
        word_width = talk_next_word(&text);
        if (((line_width + word_width) + left) < right) {
            func_000A1054((char *)text_rsc_buffer, (char *)text_buffer, str_talk_c, 1108, 2048);
            line_width += word_width;
        } else {
            talk_add_line((iptr)text_rsc_buffer);
            mc_strncpy((char *)text_rsc_buffer, (char *)text_buffer, 2048, str_talk_c, 1114);
            line_width = word_width;
        }
    }
    talk_add_line((iptr)text_rsc_buffer);
    if (talk_wrap_to_question != 0) return;
    if ((talk_answer_scroll = talk_answer_line_count - 18) >= 0) return;
    talk_answer_scroll = 0;
}

void talk_log_question(void)
{
    int i;

    talk_add_line((iptr)str_empty);
    for (i = 0; i < talk_question_line_count; i++) {
        *(signed char *)((char **)talk_question_lines)[i] |= 128;
        talk_add_line(((iptr *)talk_question_lines)[i]);
    }
    talk_clear_question();
}

void talk_add_line(iptr line)
{
    if (talk_wrap_to_question != 0) {
        ((iptr *)talk_question_lines)[talk_question_line_count] = talk_alloc_line();
        mc_strncpy((char *)((iptr *)talk_question_lines)[talk_question_line_count++], (char *)line, 4, str_talk_c, 1148);
        return;
    }
    ((iptr *)talk_answer_lines)[talk_answer_line_count] = talk_alloc_line();
    mc_strncpy((char *)((iptr *)talk_answer_lines)[talk_answer_line_count++], (char *)line, 4, str_talk_c, 1153);
}

int talk_next_word(signed char **cursor)
{
    int width;
    int length;

    width = 0;
    length = 0;
    while (((int)(unsigned char)**cursor) > 32) {
        text_buffer[length++] = **cursor;
        width += font_char_width((int)(unsigned char)**cursor);
        (*cursor)++;
    }
    if (width == 0 && **cursor != 0) {
        text_buffer[length++] = **cursor;
        width += font_char_width((int)(unsigned char)**cursor);
        (*cursor)++;
    }
    text_buffer[length] = 0;
    return width;
}

iptr talk_alloc_line(void)
{
    talk_text_pool_next += strlen((char *)talk_text_pool_next) + 1;
    return talk_text_pool_next;
}

void talk_init_text(void)
{
    if (talk_text_pool == 0) talk_text_pool = (iptr)mc_malloc(20480, str_talk_c, 1238);
    if (talk_answer_line_buffer == 0) talk_answer_line_buffer = (iptr)mc_malloc(20480, str_talk_c, 1241);
    mc_memset((void *)talk_text_pool, 0, 20480, str_talk_c, 1243, 4);
    mc_memset((void *)talk_answer_line_buffer, 0, 20480, str_talk_c, 1244, 4);
    talk_text_pool_next = talk_text_pool;
    *(iptr *)&talk_answer_lines = talk_answer_line_buffer;
    talk_answer_line_count = 0;
    talk_clear_question();
}

void talk_clear_question(void)
{
    if (talk_question_line_buffer == 0) talk_question_line_buffer = (iptr)mc_malloc(20480, str_talk_c, 1257);
    mc_memset((void *)talk_question_line_buffer, 0, 20480, str_talk_c, 1259, 4);
    *(iptr *)&talk_question_lines = talk_question_line_buffer;
    talk_question_line_count = 0;
}

void talk_draw_place_list(void)
{
    int i;
    int colour;
    int saved_clip;
    iptr name;

    saved_clip = (int)(short)xn_gfx_clip_right;
    xn_gfx_clip_right = 100;
    talk_list_count = 0;
    if (talk_topic_tab == 0) {
        if (talk_list_count >= talk_list_top && talk_list_count <= talk_list_bottom) {
            talk_list_draw_item((iptr)str_previous_list, 6, (int)(iptr)&*(signed char *)((char *)(iptr)((talk_list_count - talk_list_top) * 7) + 71), 96, 156);
        }
        talk_list_count++;
        if (((int)(unsigned char)talk_location_category) == 14) {
            talk_draw_regional_list();
        } else {
            for (i = 0; i < talk_place_topic_count; i++) {
                if (talk_place_topics[i].category != talk_location_category) continue;
                if (talk_list_count < talk_list_top || talk_list_count > talk_list_bottom) {
                    talk_list_count++;
                    continue;
                }
                name = building_name((iptr)talk_place_topics[i].building);
                if (*(int *)talk_selected_row == talk_list_count) {
                    colour = 244;
                    mc_strncpy((char *)(((iptr)talk_key_text) + (((int)(unsigned char)talk_key_index) * 65)), (char *)name, 4, str_talk_c, 1300);
                } else {
                    colour = 145;
                }
                talk_list_draw_item(name, 6, (int)(iptr)&*(signed char *)((char *)(iptr)((talk_list_count - talk_list_top) * 7) + 71), colour, 156);
                talk_place_rows[talk_list_count++] = *(signed char *)&i;
            }
        }
    } else if (((int)(unsigned char)talk_topic_tab) == 1) {
        talk_add_quest_topics(5);
    } else if (((int)(unsigned char)talk_topic_tab) == 2) {
        talk_add_quest_topics(6);
    }
    xn_gfx_clip_right = saved_clip;
}

void talk_list_click(int button)
{
    int row;

    if (button == 100 && talk_topic_tab == 0 && ((int)(unsigned char)talk_question_mode) == 1 && talk_showing_categories != 0) {
        talk_location_category = *(signed char *)talk_selected_row;
        talk_showing_categories = 0;
        *(int *)talk_selected_row = 1;
        talk_redraw = 1;
        return;
    }
    row = (int)(iptr)(((char *)(iptr)talk_list_top) + ((((int)(short)mouse_y) - 71) / 7));
    if (row >= talk_list_count) return;
    if (talk_topic_tab == 0 && ((int)(unsigned char)talk_question_mode) == 1) {
        if (talk_showing_categories != 0) {
            if (mouse_double_click != 0 || key_pressed_once(28) != 0) {
                talk_location_category = talk_category_rows[row];
                talk_showing_categories = 0;
                *(int *)talk_selected_row = 1;
            } else {
                *(int *)talk_selected_row = (int)(unsigned char)talk_category_rows[row];
            }
        } else if (key_pressed_once(1) != 0 || (row == 0 && (mouse_double_click != 0 || key_pressed_once(28) != 0))) {
            while (key_down_esc != 0);
            talk_showing_categories = 1;
        } else if (row != 0) {
            *(int *)talk_selected_row = row;
        } else {
            *(int *)talk_selected_row = 1;
        }
    } else {
        *(int *)talk_selected_row = row;
    }
    talk_redraw = 1;
}

void talk_add_quest_topics(int section)
{
    struct record *object;
    struct record *next;
    struct record *place_object;
    struct qbn_place *place;
    struct qbn_person *person;
    struct qbn_item *item;
    iptr name;
    int i;
    int colour;
    int j;
    struct building *building;

    object = quest_root->children;
    while (object != 0) {
        next = object->next;
        if (object->type == 14) {
            quest_tick_object = object;
            current_quest = (quest_tick_data = &object->data.quest);
            if (((unsigned)section) >= 5) {
                if (((unsigned)section) <= 5) goto L1776B;
                if (section == 6) goto L1790A;
            } else {
                if (section != 4) goto L17A4C;
                place = (struct qbn_place *)quest_section((iptr)quest_tick_data, 4);
                for (i = 0; quest_tick_data->section_counts[4] > i; i++, place++) {
                    if (place->object->twin == 0) continue;
                    if (((int)(unsigned char)(place->flags & 128)) != 0) continue;
                    place_object = place->object->twin;
                    if (place_object->type == 1) continue;
                    name = quest_symbol_text(place->symbol, 0, 0);
                    building = object_building(place_object);
                    if (building->type < 17 && building->type != 1) {
                        for (j = 0; j < talk_place_topic_count; j++) {
                            if (talk_place_topics[j].building == building) {
                                talk_place_topics[talk_place_topic_count].quest = (signed char)quest_tick_data->id;
                                talk_place_topics[talk_place_topic_count].messages[0] = place->messages[0];
                                talk_place_topics[talk_place_topic_count++].messages[1] = place->messages[1];
                            }
                        }
                        continue;
                    }
                    talk_list_draw_item(name, 6, ((talk_list_count - talk_list_top) * 7) + 71, colour, 156);
                    talk_place_topics[talk_place_topic_count].category = building_category(building);
                    talk_place_topics[talk_place_topic_count].distance = building_distance(building);
                    talk_place_topics[talk_place_topic_count].building = building;
                    talk_place_topics[talk_place_topic_count].kind = 1;
                    talk_place_topics[talk_place_topic_count].quest = (signed char)quest_tick_data->id;
                    talk_place_topics[talk_place_topic_count].messages[0] = place->messages[0];
                    talk_place_topics[talk_place_topic_count++].messages[1] = place->messages[1];
                }
                goto L17A4C;
L1776B:;
                person = (struct qbn_person *)quest_section((iptr)quest_tick_data, 3);
                for (i = 0; quest_tick_data->section_counts[3] > i; i++, person++) {
                    if (person->object == 0 || person->object->twin == 0) continue;
                    if ((iptr)person->object->twin == (iptr)talk_npc_object) continue;
                    if ((((int)(short)person->flags) & 32768) != 0) continue;
                    name = quest_symbol_text(person->symbol, 0, 0);
                    if (talk_list_count < talk_list_top || talk_list_count > talk_list_bottom) {
                        talk_list_count++;
                        continue;
                    }
                    if (*(int *)talk_selected_row == talk_list_count) {
                        colour = 244;
                        mc_strncpy((char *)(((iptr)talk_key_text) + (((int)(unsigned char)talk_key_index) * 65)), (char *)name, 4, str_talk_c, 1448);
                    } else {
                        colour = 145;
                    }
                    talk_list_draw_item(name, 6, (int)(iptr)&*(signed char *)((char *)(iptr)((talk_list_count - talk_list_top) * 7) + 71), colour, 156);
                    talk_place_topics[talk_list_count].building = object_building(person->object->twin);
                    talk_place_topics[talk_list_count].kind = 2;
                    talk_place_topics[talk_list_count].quest = (signed char)quest_tick_data->id;
                    talk_place_topics[talk_list_count].messages[0] = person->messages[0];
                    talk_place_topics[talk_list_count++].messages[1] = person->messages[1];
                }
                goto L17A4C;
L1790A:;
                goto L17A4C;
                for (; quest_tick_data->section_counts[0] > i; i++, item++) {
                    if (((int)(unsigned char)(item->flags & 128)) != 0) continue;
                    name = quest_symbol_text(item->symbol, 0, 0);
                    if (talk_list_count < talk_list_top || talk_list_count > talk_list_bottom) {
                        talk_list_count++;
                        continue;
                    }
                    if (*(int *)talk_selected_row == talk_list_count) {
                        colour = 244;
                        mc_strncpy((char *)(((iptr)talk_key_text) + (((int)(unsigned char)talk_key_index) * 65)), (char *)name, 4, str_talk_c, 1477);
                    } else {
                        colour = 145;
                    }
                    talk_list_draw_item(name, 6, (int)(iptr)&*(signed char *)((char *)(iptr)((talk_list_count - talk_list_top) * 7) + 71), colour, 156);
                    talk_place_topics[talk_list_count].kind = 3;
                    talk_place_topics[talk_list_count].quest = (signed char)quest_tick_data->id;
                    talk_place_topics[talk_list_count].messages[0] = item->messages[0];
                    talk_place_topics[talk_list_count++].messages[1] = item->messages[1];
                }
            }
        }
L17A4C:;
        object = next;
    }
}

void talk_list_draw_item(iptr text, int x, int y, int colour, int shadow)
{
    int width;
    slot16 saved_left;
    slot16 saved_right;

    *(int *)&saved_left = (int)(short)xn_gfx_clip_left;
    xn_gfx_clip_left = 6;
    *(int *)&saved_right = (int)(short)xn_gfx_clip_right;
    xn_gfx_clip_right = 100;
    text_draw_coloured((char *)text, (int)(short)(x + *(short *)talk_list_scroll_x), (int)(short)*(short *)&y, (int)(short)*(short *)&colour, (int)(short)*(short *)&shadow);
    width = font_text_width(text);
    if (width > talk_list_max_width) talk_list_max_width = width;
    xn_gfx_clip_left = *(int *)&saved_left;
    xn_gfx_clip_right = *(int *)&saved_right;
}

iptr talk_macro_1com(void)
{
    if (((int)(unsigned short)(*(short *)talk_flags & 2)) != 0) {
        if (talk_disposition <= 0) {
            text_macro_n_text = *(short *)(talk_greeting_texts + (((int)(unsigned char)talk_tone) << 2));
        }
        parse_rsc_text((int)(unsigned short)((short *)talk_greeting_texts)[((int)(unsigned char)talk_tone) * 2 + 1], 0, 0);
        text_macro_n_text = 0;
    } else {
        parse_rsc_text((int)(unsigned short)talk_ack_texts[((int)(unsigned char)talk_tone)], 0, 0);
    }
    return (iptr)text_rsc_buffer;
}

void talk_draw_regional_list(void)
{
    int i;

    for (i = 0; i < 28; i++) {
        if (i >= 8 && i <= 17) {
            if (current_region != talk_regional_regions[i]) continue;
            if (town_has_building((int)(short)talk_regional_ids[i], 0) == 0) {
                talk_add_regional_item(i);
            }
        } else if (i < 20) {
            if (town_has_building((int)(short)talk_regional_ids[i], 0) == 0) {
                talk_add_regional_item(i);
            }
        } else if (town_has_building((int)(short)talk_regional_ids[i], 1) == 0) {
            talk_add_regional_item(i);
        }
    }
}

void talk_add_regional_item(int index)
{
    int colour;

    if (talk_list_count < talk_list_top || talk_list_count > talk_list_bottom) {
        talk_list_count++;
        return;
    }
    mc_set_location(1590, str_talk_c);
    mc_sprintf((char *)text_buffer, str_any, talk_regional_names[index]);
    if (*(int *)talk_selected_row == talk_list_count) {
        colour = 244;
        mc_strncpy((char *)(((iptr)talk_key_text) + (((int)(unsigned char)talk_key_index) * 65)), (char *)text_buffer, 4, str_talk_c, 1595);
    } else {
        colour = 145;
    }
    talk_list_draw_item((iptr)text_buffer, 6, (int)(iptr)&*(signed char *)((char *)(iptr)((talk_list_count - talk_list_top) * 7) + 71), colour, 156);
    *(signed char *)(talk_regional_rows + talk_list_count++) = *(signed char *)&index;
}

int talk_find_regional(int index)
{
    {
        struct loaded_location found;

        location_pick_random_with_service(&found, (int)(short)talk_regional_services[index], (int)(short)talk_regional_ids[index]);
        if (found.object == 0) return 0;
        mc_strncpy(text_macro_fcn, found.data->name, 32, str_talk_c, 1610);
        location_free(&found);
        return 1;
    }
}

void talk_build_place_topics(void)
{
    struct talk_place_topic swap;
    int i;
    int last;
    int swapped;
    int unused1;
    int next_distance;
    int unused2;
    struct building *building;
    iptr found;
    int distance;

    building = current_location->buildings;
    talk_place_topic_count = 0;
    for (i = 0; current_location->building_count > i; i++, building++) {
        if (building->type >= 17 || building->type == 1) continue;
        found = (iptr)memchr(talk_category_building_types, building->type, 13);
        if (found != 0) {
            talk_place_topics[talk_place_topic_count].category = found - ((iptr)talk_category_building_types);
            talk_place_topics[talk_place_topic_count].kind = 0;
            talk_place_topics[talk_place_topic_count].distance = building_distance(building);
            talk_place_topics[talk_place_topic_count++].building = building;
        } else {
            talk_place_topics[talk_place_topic_count].category = 13;
            talk_place_topics[talk_place_topic_count].kind = 0;
            talk_place_topics[talk_place_topic_count].distance = building_distance(building);
            talk_place_topics[talk_place_topic_count++].building = building;
        }
    }
    talk_add_quest_topics(4);
    talk_place_topics[talk_place_topic_count].category = 14;
    talk_place_topics[talk_place_topic_count].kind = 0;
    talk_place_topics[talk_place_topic_count++].building = 0;
    swapped = 1;
    last = talk_place_topic_count - 2;
    if (last < 2) return;
    while (swapped != 0) {
        swapped = 0;
        for (i = 0; i < last; i++) {
            distance = talk_place_topics[i].distance;
            next_distance = talk_place_topics[i + 1].distance;
            if (next_distance < distance) {
                swapped = 1;
                mc_memcpy(&swap, &talk_place_topics[i], sizeof(swap), str_talk_c, 1678, 4);
                mc_memcpy(&talk_place_topics[i], &talk_place_topics[i + 1], sizeof(swap), str_talk_c, 1679, 4);
                mc_memcpy(&talk_place_topics[i + 1], &swap, sizeof(swap), str_talk_c, 1680, 4);
            }
        }
        last--;
    }
}

void talk_draw_categories(void)
{
    int category;
    int colour;

    talk_list_count = 0;
    if (talk_showing_categories == 0) return;
    for (category = 0; category < 15; category++) {
        if (town_has_category(category) != 0 || category == 14) {
            if (*(int *)talk_selected_row == category) {
                colour = 244;
            } else {
                colour = 145;
            }
            talk_list_draw_item(talk_category_names[category], 6, (int)(iptr)&*(signed char *)((char *)(iptr)((talk_list_count - talk_list_top) * 7) + 71), colour, 156);
            talk_category_rows[talk_list_count++] = *(signed char *)&category;
        }
    }
}

iptr building_category(struct building *building)
{
    iptr found;

    if (building->type != 1 && building->type <= 16) {
        found = (iptr)memchr(talk_category_building_types, building->type, 13);
        if (found == 0) return 13;
        return found - ((iptr)talk_category_building_types);
    }
    if (((int)(unsigned char)(building->flags & 8)) != 0) return 13;
    return -1;
}

int town_has_category(iptr category)
{
    int i;
    struct building *building;

    building = current_location->buildings;
    for (i = 0; current_location->building_count > i; i++, building++) {
        if (building_category(building) == category) return 1;
    }
    return 0;
}

void talk_mark_quest_buildings(void)
{
    int i;
    struct building *building;

    building = current_location->buildings;
    for (i = 0; current_location->building_count > i; i++, building++) {
        if (quest_find_site_for_building(building) != 0) building->flags |= 8;
    }
}

void talk_add_quest_info_topics(void)
{
    struct record *object;
    struct record *next;
    struct qbn_place *place;
    struct qbn_person *person;
    struct qbn_item *item;
    iptr name;
    int i;
    int colour;
    int unused;

    object = quest_root->children;
    while (object != 0) {
        next = object->next;
        if (object->type == 14) {
            quest_tick_object = object;
            current_quest = (quest_tick_data = &object->data.quest);
            place = (struct qbn_place *)quest_section((iptr)quest_tick_data, 4);
            for (i = 0; quest_tick_data->section_counts[4] > i; i++, place++) {
                if (place->messages[0] == 0 && place->messages[1] == 0) continue;
                if (((int)(unsigned char)(place->flags & 128)) != 0) continue;
                *(short *)((((char *)talk_topics) + (((int)(short)talk_topic_count) * 6))) = place->messages[0];
                talk_topics[((int)(short)talk_topic_count) * 3 + 1] = place->messages[1];
                talk_topics[((int)(short)talk_topic_count) * 3 + 2] = quest_tick_data->id;
                talk_topic_count++;
                if (talk_list_count < talk_list_top || talk_list_count > talk_list_bottom) {
                    talk_list_count++;
                    continue;
                }
                name = quest_symbol_text(place->symbol, 0, 0);
                if (*(int *)talk_selected_row == talk_list_count) {
                    colour = 244;
                    mc_strncpy((char *)(((iptr)talk_key_text) + (((int)(unsigned char)talk_key_index) * 65)), (char *)name, 4, str_talk_c, 1790);
                } else {
                    colour = 145;
                }
                talk_list_draw_item(name, 6, (int)(iptr)&*(signed char *)((char *)(iptr)((talk_list_count - talk_list_top) * 7) + 71), colour, 156);
                talk_list_count++;
            }
            person = (struct qbn_person *)quest_section((iptr)quest_tick_data, 3);
            for (i = 0; quest_tick_data->section_counts[3] > i; i++, person++) {
                if (person->messages[0] == 0 && person->messages[1] == 0) continue;
                if ((((int)(short)person->flags) & 32768) != 0) continue;
                if (person->object->twin != 0 && (iptr)person->object->twin == (iptr)talk_npc_object) {
                    continue;
                }
                *(short *)((((char *)talk_topics) + (((int)(short)talk_topic_count) * 6))) = person->messages[0];
                talk_topics[((int)(short)talk_topic_count) * 3 + 1] = person->messages[1];
                talk_topics[((int)(short)talk_topic_count) * 3 + 2] = quest_tick_data->id;
                talk_topic_count++;
                if (talk_list_count < talk_list_top || talk_list_count > talk_list_bottom) {
                    talk_list_count++;
                    continue;
                }
                name = quest_symbol_text(person->symbol, 0, 0);
                if (*(int *)talk_selected_row == talk_list_count) {
                    colour = 244;
                    mc_strncpy((char *)(((iptr)talk_key_text) + (((int)(unsigned char)talk_key_index) * 65)), (char *)name, 4, str_talk_c, 1821);
                } else {
                    colour = 145;
                }
                talk_list_draw_item(name, 6, (int)(iptr)&*(signed char *)((char *)(iptr)((talk_list_count - talk_list_top) * 7) + 71), colour, 156);
                talk_list_count++;
            }
            item = (struct qbn_item *)quest_section((iptr)quest_tick_data, 0);
            for (i = 0; quest_tick_data->section_counts[0] > i; i++, item++) {
                if (item->messages[0] == 0 && item->messages[1] == 0) continue;
                if (((int)(unsigned char)(item->flags & 128)) != 0) continue;
                if (item->group == 9 && item->index == 5) continue;
                *(short *)((((char *)talk_topics) + (((int)(short)talk_topic_count) * 6))) = item->messages[0];
                talk_topics[((int)(short)talk_topic_count) * 3 + 1] = item->messages[1];
                talk_topics[((int)(short)talk_topic_count) * 3 + 2] = quest_tick_data->id;
                talk_topic_count++;
                if (talk_list_count < talk_list_top || talk_list_count > talk_list_bottom) {
                    talk_list_count++;
                    continue;
                }
                name = quest_symbol_text(item->symbol, 0, 0);
                if (*(int *)talk_selected_row == talk_list_count) {
                    colour = 244;
                    mc_strncpy((char *)(((iptr)talk_key_text) + (((int)(unsigned char)talk_key_index) * 65)), (char *)name, 4, str_talk_c, 1852);
                } else {
                    colour = 145;
                }
                talk_list_draw_item(name, 6, (int)(iptr)&*(signed char *)((char *)(iptr)((talk_list_count - talk_list_top) * 7) + 71), colour, 156);
                talk_list_count++;
            }
        }
        object = next;
    }
}

int talk_faction_greeting(short faction_id)
{
    int relation;
    int roll;
    pslot16 faction;

    *(iptr *)&faction = (iptr)faction_find((int)(short)faction_id);
    if (((int)(unsigned char)*(signed char *)(*(char **)&faction)) == 15 || ((int)(unsigned char)*(signed char *)(*(char **)&faction)) == 14) {
        return 0;
    }
    relation = talk_faction_relation((int)(short)faction_id);
    talk_faction_disposition += *(short *)(*(char **)&faction + 29);
    if (((int)(unsigned char)*(signed char *)(*(char **)&faction + 54)) < 5) {
        talk_faction_disposition += player_character->reputation[(int)(unsigned char)*(signed char *)(*(char **)&faction + 54)];
    }
    roll = rand_range(0, 15) - 10;
    if (((int)(short)talk_faction_disposition) >= roll && ((int)(short)*(short *)(*(char **)&faction + 29)) >= 30) {
        return (int)(short)*(short *)(talk_faction_greetings + (relation * 6));
    }
    if (((int)(short)talk_faction_disposition) >= roll) {
        return (int)(short)*(short *)(str_g_h_i_j_k_l_m_n_o_p_q_r_r_s_t_t_u_v_v_w + (relation * 6));
    }
    talk_refused = 1;
    return (int)(short)*(short *)(str_h_i_j_k_l_m_n_o_p_q_r_r_s_t_t_u_v_v_w_x + (relation * 6));
}

int talk_roll_knows(void)
{
    int knows;
    int question_kind;
    int result;
    int seed;

    seed = rand();
    if (talk_npc_own_faction->id == 806 || talk_npc_own_faction->id == 842) return 1;
    question_kind = ((1 - ((int)(unsigned char)talk_question_mode)) << 2) + ((int)(unsigned char)talk_topic_tab);
    srand(((talk_npc_object->id ^ (((unsigned)talk_npc_object->id) >> 16)) + question_kind) + *(int *)talk_selected_row);
    if (rand_range(1, 20) <= (((int)(short)*(short *)(talk_knows_mods + ((question_kind * 10) + (talk_npc_speech_style * 2)))) + 10)) {
        knows = 1;
    } else {
        knows = 0;
    }
    result = knows;
    srand(seed);
    return result;
}

int talk_roll_attitude(void)
{
    int attitude;
    int roll;

    attitude = 0;
    if (talk_tone == 0) {
        if (rand_range(1, 100) < player_character->skills[1].value) {
            attitude += 10;
        } else {
            attitude += -5;
        }
        if (talk_attitude_cache[0] == 0) skill_add_uses(1, 1);
    }
    if (((int)(unsigned char)talk_tone) == 2) {
        if (rand_range(1, 100) < player_character->skills[2].value) {
            attitude += 10;
        } else {
            attitude += -5;
        }
        if (talk_etiquette_counted == 0) skill_add_uses(2, 1);
    }
    attitude += (int)(short)talk_question_attitude_mods[(((1 - ((int)(unsigned char)talk_question_mode)) << 2) + ((int)(unsigned char)talk_topic_tab))];
    attitude += player_character->attributes[5] / 5;
    roll = rand_range(0, 20);
    if (talk_attitude_cache[((int)(unsigned char)talk_tone)] != 0) {
        attitude = talk_attitude_cache[((int)(unsigned char)talk_tone)];
    } else {
        talk_attitude_cache[((int)(unsigned char)talk_tone)] = attitude;
    }
    if (attitude < roll) return 0;
    if ((roll + 30) <= attitude) return 2;
    return 1;
}

struct faction *faction_find_type_in_region_r(struct faction *faction, short region, short type)
{
    struct faction *found;

    while (faction != 0) {
        if ((short)((unsigned short)faction->type) == type && faction->region == (-1)) {
            found_faction = faction;
        } else if ((short)((unsigned short)faction->type) == type && (short)((unsigned short)faction->region) == region) {
            return faction;
        }
        if (faction->child != 0) {
            found = faction_find_type_in_region_r(faction->child, (int)(short)region, (int)(short)type);
            if (found != 0) return found;
        }
        faction = faction->next;
    }
    return 0;
}

struct faction *faction_find_r(struct faction *faction, short id)
{
    pslot16 found;

    while (faction != 0) {
        if (faction->id == (short)id) return faction;
        if (faction->child != 0) {
            *(iptr *)&found = (iptr)faction_find_r(faction->child, (int)(short)id);
            if (*(iptr *)&found != 0) return (struct faction *)*(iptr *)&found;
        }
        faction = faction->next;
    }
    return 0;
}

void faction_relation_flags_r(struct faction *faction)
{
    int unused;
    int i;

    while (faction != 0) {
        if ((iptr)faction == *(iptr *)scratch_190be4) *(signed char *)shared_scratch |= 1;
        for (i = 0; i < 3; i++) {
            if ((iptr)faction->allies[i] == *(iptr *)scratch_190be4) *(signed char *)shared_scratch |= 1;
            if ((iptr)faction->enemies[i] == *(iptr *)scratch_190be4) *(signed char *)shared_scratch |= 2;
        }
        if (faction->child != 0) faction_relation_flags_r(faction->child);
        faction = faction->next;
    }
}

struct faction *faction_random(void)
{
    *(int *)shared_scratch = rand_range(0, faction_count - 1);
    return faction_nth_r(factions);
}

struct faction *faction_nth_r(struct faction *faction)
{
    struct faction *found;

    while (faction != 0) {
        if (*(int *)shared_scratch == 0) return faction;
        (*(int *)shared_scratch)--;
        if (faction->child != 0) {
            found = faction_nth_r(faction->child);
            if (found != 0) return found;
        }
        faction = faction->next;
    }
    return 0;
}

struct faction *faction_nth_of_type_r(struct faction *faction, unsigned char type)
{
    pslot16 found;

    while (faction != 0) {
        if (*(int *)shared_scratch == 0 && faction->type == type) return faction;
        if (faction->child != 0) {
            *(iptr *)&found = (iptr)faction_nth_of_type_r(faction->child, (int)(unsigned char)type);
            if (*(iptr *)&found != 0) return (struct faction *)*(iptr *)&found;
        }
        if (faction->type == type) (*(int *)shared_scratch)--;
        faction = faction->next;
    }
    return 0;
}

void faction_count_of_type_r(struct faction *faction, unsigned char type)
{
    int unused;

    while (faction != 0) {
        if (faction->type == type) (*(int *)shared_scratch)++;
        if (faction->child != 0) faction_count_of_type_r(faction->child, (int)(unsigned char)type);
        faction = faction->next;
    }
}

struct faction *faction_random_of_type(unsigned char type)
{
    *(int *)shared_scratch = 0;
    faction_count_of_type_r(factions, (int)(unsigned char)type);
    *(int *)shared_scratch = rand_range(0, *(int *)shared_scratch - 1);
    return faction_nth_of_type_r(factions, (int)(unsigned char)type);
}

int factions_can_war(struct faction *faction1, struct faction *faction2)
{
    {
        int result;

        (*(int *)shared_scratch)++;
        if (faction_is_regional_noble(faction1) != 0 && faction_is_regional_noble(faction2) != 0 && faction_has_enemy(faction1, faction2) != 0 && faction_regions_border(faction1, faction2) != 0) {
            result = 1;
        } else {
            result = 0;
        }
        return result;
    }
}

int faction_is_regional_noble(struct faction *faction)
{
    /* an empty enemy slot (0) reads the zero page under DOS: not a noble */
    return (((DOS_NULL(faction)->type == 7) && (DOS_NULL(faction)->region != 255)) ? 1 : 0);
}

int faction_has_enemy(struct faction *faction, struct faction *other)
{
    {
        int result;

        if (other == 0) return 0;
        if (faction == 0) return 0;
        if (faction->enemies[0] == other || faction->enemies[1] == other || faction->enemies[2] == other) {
            result = 1;
        } else {
            result = 0;
        }
        return result;
    }
}

int faction_has_ally(struct faction *faction, struct faction *other)
{
    {
        int result;

        if (faction == 0) return 0;
        if (other == 0) return 0;
        if (faction->allies[0] == other || faction->allies[1] == other || faction->allies[2] == other) {
            result = 1;
        } else {
            result = 0;
        }
        return result;
    }
}

void faction_add_power(struct faction *faction, int amount)
{
    if (faction == 0) return;
    faction->power += amount;
    if (faction->power > 100) {
        faction->power = 100;
        return;
    }
    if (faction->power >= 0) return;
    faction->power = 1;
}

void faction_max_power_r(struct faction *faction)
{
    while (faction != 0) {
        if (faction->child != 0) faction_max_power_r(faction->child);
        if (faction->power > *(int *)shared_scratch) *(int *)shared_scratch = faction->power;
        faction = faction->next;
    }
}

int faction_max_child_power(struct faction *faction)
{
    *(int *)shared_scratch = 0;
    faction_max_power_r(faction->child);
    return *(int *)shared_scratch;
}

int faction_shared_relations(struct faction *faction1, struct faction *faction2)
{
    int i;
    int j;
    int shared;

    shared = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (faction_has_ally(faction1->allies[i], faction2->allies[j]) != 0) shared += 3;
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (faction_has_enemy(faction1->enemies[i], faction2->enemies[j]) != 0) shared += 3;
        }
    }
    return *(int *)shared_scratch;
}

int faction_subtree_search_r(struct faction *faction)
{
    if (*(int *)shared_scratch != 0) return *(int *)shared_scratch;
    while (faction != 0) {
        if ((iptr)faction == (iptr)macro_faction2) *(signed char *)shared_scratch |= 2;
        if (faction == found_faction) {
            *(signed char *)shared_scratch |= 1;
            return *(int *)shared_scratch;
        }
        if (faction->child != 0 && ((struct bf8_1_1 *)&shared_scratch)->f == 0) faction_subtree_search_r(faction->child);
        faction = faction->next;
    }
    return *(int *)shared_scratch;
}

int faction_regions_border(struct faction *faction1, struct faction *faction2)
{
    iptr neighbours;
    int i;
    int region;

    if (faction1 != 0 && faction1->region != 255 && faction2 != 0 && faction2->region != 255) {
        neighbours = ((iptr)region_neighbours) + (faction1->region * 11);
        region = faction2->region;
        for (i = 0; i < 11; i++) {
            if (((int)(unsigned char)*(signed char *)((char *)(neighbours + i))) == region) return 1;
        }
    }
    return 0;
}

int faction_power(struct faction *faction)
{
    if (faction != 0) return faction->power;
    return 0;
}

void region_reset_war(struct faction *faction)
{
    if (faction->type != 7 || faction->region == 255) return;
    regions[faction->region].groups[0] = 0;
    regions[faction->region].values[0] = 0;
    regions[faction->region].flags[0] = 0;
    regions[faction->region].flags[1] = 0;
    regions[faction->region].flags[2] = 0;
    regions[faction->region].flags[3] = 0;
}

void region_set_war_ongoing(struct faction *faction)
{
    if (faction->type != 7 || faction->region == 255) return;
    regions[faction->region].groups[0] = 1;
    regions[faction->region].values[1] = 255;
    regions[faction->region].flags[1] = 1;
}

void region_set_war_result(struct faction *faction, int index)
{
    if (faction->type != 7 || faction->region == 255) return;
    regions[faction->region].values[0] = 0;
    regions[faction->region].values[1] = 0;
    regions[faction->region].values[index + 2] = 0;
    regions[faction->region].flags[index + 2] = 1;
    regions[faction->region].groups[0] = 1;
}

void faction_add_reputation(struct faction *faction, int amount)
{
    if (faction == 0) return;
    faction->reputation += amount;
    if (faction->reputation > 100) {
        faction->reputation = 100;
        return;
    }
    if (faction->reputation >= (-100)) return;
    faction->reputation = 65436;
}

void faction_change_reputation(struct faction *faction, int amount)
{
    int i;
    int unused;
    struct membership *membership;

    if (faction == 0) return;
    if (faction->id == 240) {
        membership = guild_find_membership_by_bits(128);
        if (membership != 0) faction = faction_find(membership->faction);
    }
    faction_propagate_reputation(faction, amount);
    for (i = 0; i < 3; i++) {
        if (faction->allies[i] != 0) faction_add_reputation(faction->allies[i], amount >> 1);
    }
    for (i = 0; i < 3; i++) {
        if (faction->enemies[i] != 0) faction_add_reputation(faction->enemies[i], -(amount >> 1));
    }
}

void faction_add_reputation_r(struct faction *faction, struct faction *target, int amount)
{
    while (faction != 0) {
        if (faction == target) {
            faction_add_reputation(faction, amount);
        } else {
            faction_add_reputation(faction, amount >> 1);
        }
        if (faction->child != 0) faction_add_reputation_r(faction->child, target, amount);
        faction = faction->next;
    }
}

void faction_propagate_reputation(struct faction *faction, int amount)
{
    struct faction *original;
    struct membership *membership;

    original = faction;
    while (faction->parent != 0) {
        if (faction->id == 108) {
            faction_add_reputation(faction, amount);
            break;
        }
        faction = faction->parent;
    }
    if (faction->parent == 0 && faction->id != 844) faction_add_reputation(faction, amount);
    if (faction->id == 844) {
        faction_add_reputation(faction, amount);
        membership = guild_find_membership_by_bits(64);
        if (membership != 0) faction = faction_find(membership->faction);
        if (faction != 0) faction_add_reputation(faction, amount);
    }
    faction_add_reputation_r(faction->child, original, amount);
}
