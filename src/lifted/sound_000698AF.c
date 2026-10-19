/* sound.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */

extern int sound_volume;
extern int sos_midi_device;
extern signed char sound_enabled;

#include "clib.h"

void sound_set_volume(int volume)
{
    if (sound_enabled == 0) return;
    if (sos_midi_device == (-1)) return;
    func_000A1D3C(volume);
    sound_volume = volume;
}
