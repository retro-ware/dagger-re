/* trade.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"

extern void xn_timer_tick_callback(void);
extern signed char text_colour;
extern char str_trade_c[];
extern char str_the_current_amount_in_the_offering_is[];
extern char trade_counter_prompt_fmt[];
extern double haggle_quality_factor;
extern double haggle_base_step;
extern double haggle_skill_factor;
extern double haggle_factor_4;
extern char str_hmiset_cfg[];
extern char str_midi_bsa[];
extern int music_buffer_size;
extern int sos_midi_device;
extern int sos_digi_driver;
extern signed char text_buffer[];
extern struct building *current_building;
extern char *scratch_buffer;
extern signed char msgbox_button_keys;
extern signed char msgbox_key_2;
extern signed char msgbox_hotkey;
extern signed char msgbox_button_ids;
extern signed char msgbox_button_2;
extern signed char msgbox_button_3;
extern unsigned char popup_answer;
extern double trade_haggle_minimum;
extern double trade_haggle_asking;
extern double trade_haggle_low;
extern double trade_haggle_high;
extern double trade_haggle_step_size;
extern int trade_haggle_last_offer;
extern struct character *trade_haggle_buyer;
extern struct sound_channel sound_channels[];
extern int music_timer_handle;
extern int timer_tick_count;
extern int midi_bsa;
extern signed char sound_enabled;

#pragma aux sosconv "*" parm caller [] value [eax] modify [eax ebx ecx edx];
extern int sos_init(int, int);
#pragma aux (sosconv) sos_init;
extern int sos_shutdown(void);
extern int sos_read_settings(char *);
#pragma aux (sosconv) sos_read_settings;
extern int archive_open(char *, iptr, int);
extern int sound_timer_add(iptr, int);
extern int dpmi_lock_region(iptr, int);
extern int dpmi_unlock_region(iptr, int);
extern void archive_close(int);
extern void msgbox_show_string(char *, short);
extern void sound_stop_channel(int);
extern void music_stop(void);
extern void sound_timer_remove(int);
extern void inpstr_begin_number(int);
void trade_haggle_step(void);
#pragma aux mc_set_location parm routine [];

void trade_haggle_show_offer(void)
{
    popup_answer = 0;
    text_colour = 146;
    msgbox_button_ids = 1;
    msgbox_button_2 = 2;
    msgbox_button_3 = 12;
    msgbox_button_keys = 30;
    msgbox_key_2 = 19;
    msgbox_hotkey = 46;
    mc_set_location(131, str_trade_c);
    mc_sprintf((char *)text_buffer, str_the_current_amount_in_the_offering_is, (int)trade_haggle_asking);
    msgbox_show_string(text_buffer, 5);
}

void trade_counter_offer(void)
{
    char *text;

    text_colour = 146;
    text = scratch_buffer + 55000;
    mc_set_location(141, str_trade_c);
    mc_sprintf(text, trade_counter_prompt_fmt);
    *(strlen(text) + text + 1) = 0;
    msgbox_show_string(text, 2);
    inpstr_begin_number((int)trade_haggle_asking);
}

int trade_haggle_counter(int offer)
{
    if (offer == trade_haggle_last_offer) return 0;
    trade_haggle_last_offer = offer;
    trade_haggle_high = trade_haggle_asking - trade_haggle_step_size;
    trade_haggle_low = trade_haggle_minimum + trade_haggle_step_size;
    if (trade_haggle_high < trade_haggle_low && offer > trade_haggle_minimum) return offer;
    if (offer > trade_haggle_high) return offer;
    if (offer < trade_haggle_minimum) return -1;
    if (offer <= trade_haggle_low && offer >= trade_haggle_minimum) return 0;
    if (offer < trade_haggle_high && offer > trade_haggle_low) {
        trade_haggle_asking -= trade_haggle_step_size;
        trade_haggle_minimum = trade_haggle_step_size + trade_haggle_minimum;
    }
    if (((int)trade_haggle_asking) == offer) return offer;
    trade_haggle_step();
    return 0;
}

void trade_haggle_step(void)
{
    trade_haggle_step_size = ((((haggle_base_step - ((short)(current_building->quality) * haggle_quality_factor)) + (trade_haggle_buyer->skills[21].value * haggle_skill_factor)) + (trade_haggle_buyer->attributes[5] * haggle_skill_factor)) + (trade_haggle_buyer->reputation[1] * haggle_factor_4)) * (trade_haggle_asking - trade_haggle_minimum);
}

int sound_init_music(void)
{
    int i;

    if ((short)sos_read_settings(str_hmiset_cfg) == 0) return 0;
    if (sos_init(sos_digi_driver, sos_midi_device) != 0) return 0;
    music_buffer_size = 2048;
    func_000A1D3C(127);
    midi_bsa = archive_open(str_midi_bsa, 0, 0);
    for (i = 0; i < 4; i++) {
        sound_channels[i].handle = 305419896;
    }
    sound_enabled = 1;
    dpmi_lock_region((iptr)sound_channels, 4 * REC_SIZEOF(struct sound_channel) + 4096);
    dpmi_lock_region((iptr)xn_timer_tick_callback, 4096);
    dpmi_lock_region((iptr)&timer_tick_count, 4096);
    music_timer_handle = sound_timer_add((iptr)xn_timer_tick_callback, 140);
    return 1;
}

void sound_shutdown_music(void)
{
    int channel;

    if (sound_enabled == 0) return;
    sound_timer_remove(music_timer_handle);
    music_stop();
    archive_close(midi_bsa);
    for (channel = 0; channel < 4; channel++) {
        sound_stop_channel(channel);
    }
    sos_shutdown();
    dpmi_unlock_region((iptr)sound_channels, 4 * REC_SIZEOF(struct sound_channel) + 4096);
    dpmi_unlock_region((iptr)xn_timer_tick_callback, 4096);
    dpmi_unlock_region((iptr)&timer_tick_count, 4096);
}
