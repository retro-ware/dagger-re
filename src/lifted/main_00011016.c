/* main.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "ptrint.h"
#include "clib.h"

extern char str_melodic_bnk[];
extern char str_drum_bnk[];
extern iptr music_song_buffer;
extern char sos_digi_settings[];
extern int sos_digi_rate;
extern int sos_digi_buffer_size;
extern int sos_digi_device;
extern iptr sos_digi_timer_callback;
extern iptr sos_drum_bank;
extern iptr sos_melodic_bank;
extern char sos_digi_lock_4[];
extern char sos_digi_timer_handle[];
extern int D_0018DD54;
extern int sos_midi_driver;
extern int sos_digi_driver;
extern int sos_digi_handle;
extern char sos_midi_settings[];
extern signed char music_uses_fm;

extern int sos_shutdown(void);
extern iptr sos_load_file(char *);
#pragma aux sosconv "*" parm caller [] value [eax] modify [eax ebx ecx edx];
#pragma aux (sosconv) sos_load_file;
extern int dpmi_lock_region(iptr, int);
#pragma aux (sosconv) sos_init;

int sos_init(int digi_device, int midi_device)
{
    sos_digi_rate = 11025;
    sos_digi_buffer_size = 4096;
    func_0009E1A1(65280, 0);
    func_0009E8FF(0, 0);
    func_0009E9C2(0, 0);
    if (midi_device != (-1)) {
        *(int *)sos_midi_settings = midi_device;
        if (func_0009EC82(sos_midi_settings, &sos_midi_driver) != 0) {
            func_0009F9A7(sos_digi_handle, 1, 1);
            func_0009E95B();
            func_0009EC0A();
            return 2;
        }
    }
    if (digi_device != (-1)) {
        sos_digi_device = digi_device;
        if (func_0009F4DE(sos_digi_settings, &sos_digi_handle) != 0) {
            func_0009E95B();
            return 1;
        }
    }
    if (digi_device != (-1)) func_0009E2BB(90, (void (*)(void))sos_digi_timer_callback, (int *)sos_digi_timer_handle);
    if (midi_device == 40962 || (midi_device == 40969 && midi_device != (-1))) {
        music_uses_fm = 1;
        if ((sos_melodic_bank = sos_load_file(str_melodic_bnk)) == 0) {
            sos_shutdown();
            return 3;
        }
        if ((sos_drum_bank = sos_load_file(str_drum_bnk)) == 0) {
            sos_shutdown();
            return 4;
        }
        if (func_0009FEE5(sos_midi_driver, (void __far *)(void *)sos_melodic_bank, 1) != 0) {
            sos_shutdown();
            return 5;
        }
        if (func_0009FEE5(sos_midi_driver, (void __far *)(void *)sos_drum_bank, 1) != 0) {
            sos_shutdown();
            return 5;
        }
    }
    dpmi_lock_region((iptr)sos_digi_settings, 268);
    dpmi_lock_region((iptr)sos_midi_settings, 46);
    dpmi_lock_region((iptr)&sos_digi_driver, 4);
    dpmi_lock_region((iptr)&sos_digi_handle, 4);
    dpmi_lock_region((iptr)sos_digi_timer_handle, 4);
    dpmi_lock_region((iptr)&D_0018DD54, 4);
    dpmi_lock_region((iptr)&sos_midi_driver, 4);
    dpmi_lock_region((iptr)sos_digi_lock_4, 4);
    dpmi_lock_region((iptr)&music_song_buffer, 4);
    dpmi_lock_region((iptr)&sos_melodic_bank, 4);
    dpmi_lock_region((iptr)&sos_drum_bank, 4);
    dpmi_lock_region(sos_melodic_bank, 8192);
    dpmi_lock_region(sos_drum_bank, 8192);
    return 0;
}
