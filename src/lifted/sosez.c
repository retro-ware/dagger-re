/* sosez.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "structs.h"
#include "clib.h"

extern char str_sosez_c[];
extern char str_riff[];
extern iptr music_song_buffer;
extern char sos_digi_settings[];
extern iptr sos_drum_bank;
extern iptr sos_melodic_bank;
extern char sos_digi_lock_4[];
extern char sos_digi_timer_handle[];
extern int sos_midi_device;
extern int sos_midi_driver;
extern int sos_digi_device;
extern int sos_digi_driver;
extern char sos_midi_settings[];
extern iptr music_song_data;
extern int music_song_locked_size;

extern int dpmi_unlock_region(iptr, int);
#pragma aux sosconv "*" parm caller [] value [eax] modify [eax ebx ecx edx];
#pragma aux (sosconv) sos_shutdown;
#pragma aux (sosconv) sos_load_sample;

int sos_shutdown(void)
{
    func_0009E61A(*(int *)sos_digi_timer_handle);
    if (sos_digi_device != (-1)) func_0009F9A7(sos_digi_driver, 1, 1);
    if (sos_midi_device != (-1)) func_0009F253(sos_midi_driver, 1);
    func_0009EC0A();
    func_0009E95B();
    func_0009E281(0);
    dpmi_unlock_region((iptr)sos_digi_settings, 268);
    dpmi_unlock_region((iptr)sos_midi_settings, 46);
    dpmi_unlock_region((iptr)&sos_digi_device, 4);
    dpmi_unlock_region((iptr)&sos_digi_driver, 4);
    dpmi_unlock_region((iptr)sos_digi_timer_handle, 4);
    dpmi_unlock_region((iptr)&sos_midi_device, 4);
    dpmi_unlock_region((iptr)&sos_midi_driver, 4);
    dpmi_unlock_region((iptr)sos_digi_lock_4, 4);
    dpmi_unlock_region((iptr)&music_song_buffer, 4);
    dpmi_unlock_region((iptr)&sos_melodic_bank, 4);
    dpmi_unlock_region((iptr)&sos_drum_bank, 4);
    if (sos_melodic_bank != 0) {
        dpmi_unlock_region(sos_melodic_bank, 8192);
        if (sos_melodic_bank != 0 && sos_melodic_bank != (-1751672937)) {
            mc_free((void *)sos_melodic_bank, str_sosez_c, 191);
            sos_melodic_bank = -1751672937;
        }
    }
    if (sos_drum_bank != 0) {
        dpmi_unlock_region(sos_drum_bank, 8192);
        if (sos_drum_bank != 0 && sos_drum_bank != (-1751672937)) {
            mc_free((void *)sos_drum_bank, str_sosez_c, 197);
            sos_drum_bank = -1751672937;
        }
    }
    if (music_song_buffer != 0) {
        dpmi_unlock_region(music_song_data, music_song_locked_size);
        if (music_song_data != 0 && music_song_data != (-1751672937)) {
            mc_free((void *)music_song_data, str_sosez_c, 203);
            music_song_data = -1751672937;
        }
    }
    return 1;
}

iptr sos_load_sample(char *path)
{
    int handle;
    int size;
    iptr buffer;
    struct sos_sample *sample;
    struct wav_header *wav;

    handle = open(path, 512);
    if (handle == (-1)) return 0;
    size = lseek(handle, 0, 2);
    lseek(handle, 0, 0);
    buffer = (iptr)mc_malloc(size + (int)sizeof(struct sos_sample), str_sosez_c, 302);
    if (buffer == 0) {
        close(handle);
        return 0;
    }
    if (read(handle, (void *)(buffer + (int)sizeof(struct sos_sample)), size) != size) {
        close(handle);
        if (buffer != 0 && buffer != (-1751672937)) {
            mc_free((void *)buffer, str_sosez_c, 318);
            buffer = -1751672937;
        }
        return 0;
    }
    close(handle);
    mc_memset((void *)buffer, 0, sizeof(struct sos_sample), str_sosez_c, 328, 4);
    sample = (struct sos_sample *)buffer;
    if (strncmp((char *)(buffer + (int)sizeof(struct sos_sample)), str_riff, 4) == 0) {
        wav = (struct wav_header *)(buffer + (int)sizeof(struct sos_sample));
        sample->data = (char *)(buffer + (int)sizeof(struct sos_sample) + (int)sizeof(struct wav_header));
        sample->length = wav->data_size - 44;
        sample->bits = wav->bits;
        sample->channels = wav->channels;
        if (wav->bits == 8) {
            sample->format = 32768;
        } else {
            sample->format = 0;
        }
        sample->rate = wav->rate;
    } else {
        sample->data = (char *)(buffer + (int)sizeof(struct sos_sample));
        sample->length = size;
        sample->bits = 8;
        sample->channels = 1;
        sample->format = 32768;
        sample->rate = 11025;
    }
    sample->pan = 32768;
    sample->volume = 2147450879;
    return (iptr)sample;
}
