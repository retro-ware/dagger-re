/* sosez.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "structs.h"
#include "clib.h"

extern char str_sosez_c[];
extern char str_digital[];
extern char str_deviceid[];
extern char str_deviceport[];
extern char str_devicedma[];
extern char str_deviceirq[];
extern char str_midi[];
extern char sos_digi_settings[];
extern char sos_digi_port[];
extern char sos_digi_irq[];
extern char sos_digi_dma[];
extern int sos_midi_device;
extern int sos_digi_driver;
extern char sos_midi_settings[];
extern char sos_midi_port[];

extern int profile_open(struct profile *, char *);
#pragma aux sosconv "*" parm caller [] value [eax] modify [eax ebx ecx edx];
#pragma aux (sosconv) profile_open;
extern int profile_close(struct profile *);
#pragma aux (sosconv) profile_close;
extern int profile_find_section(struct profile *, char *);
#pragma aux (sosconv) profile_find_section;
extern int profile_get_item_number(struct profile *, char *, int *);
#pragma aux (sosconv) profile_get_item_number;
#pragma aux (sosconv) sos_read_settings;

int sos_read_settings(char *path)
{
    struct profile profile;

    mc_memset(sos_digi_settings, 0, 268, str_sosez_c, 217, 4);
    mc_memset(sos_midi_settings, 0, 46, str_sosez_c, 218, 4);
    if ((short)profile_open(&profile, path) == 0) return 0;
    if ((short)profile_find_section(&profile, str_digital) == 0) {
        profile_close(&profile);
        return 0;
    }
    profile.result = profile_get_item_number(&profile, str_deviceid, &sos_digi_driver);
    profile.result = profile_get_item_number(&profile, str_deviceport, (int *)sos_digi_port);
    profile.result = profile_get_item_number(&profile, str_devicedma, (int *)sos_digi_dma);
    profile.result = profile_get_item_number(&profile, str_deviceirq, (int *)sos_digi_irq);
    if ((short)profile.result == 0) {
        profile_close(&profile);
        return 0;
    }
    if ((short)profile_find_section(&profile, str_midi) == 0) {
        profile_close(&profile);
        return 0;
    }
    profile.result = profile_get_item_number(&profile, str_deviceid, &sos_midi_device);
    profile.result = profile_get_item_number(&profile, str_deviceport, (int *)sos_midi_port);
    if ((short)profile.result == 0) {
        profile_close(&profile);
        return 0;
    }
    profile_close(&profile);
    return 1;
}
