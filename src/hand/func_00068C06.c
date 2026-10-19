/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00068C06 */
#include "records.h"
#include "clib.h"

extern int sos_digi_handle;
extern struct record *player_object;
extern struct career *player_class;
extern struct sound_channel sound_channels[];
extern iptr nearest_fire;
extern int sound_pan_range;
extern int sound_last_channel;
extern int sound_listener_yaw;
extern char sound_enabled;
extern void sound_volume_pan(int *, int *, int *, int *, struct record *);
extern void sound_stop_channel(int);

void sound_update_channels(void)
{
    int i;
    int volume;
    int pan;
    int unused;

    if (sound_enabled == 0)
        return;
    for (i = 0; i < 4; i++) {
        sound_last_channel = i;
        if (sound_channels[i].handle == 0x12345678)
            continue;
        if (func_000A2460(sos_digi_handle, sound_channels[i].handle) != 0) {
            sound_channels[i].handle = 0x12345678;
            continue;
        }
        if (sound_channels[i].source == 0)
            continue;
        if (sound_channels[i].source == (struct record *)nearest_fire)
            sound_pan_range = 320;
        else if ((int)(unsigned short)(player_class->flags & 1) != 0)
            sound_pan_range = 1024;
        else
            sound_pan_range = 768;
        sound_volume_pan(sound_channels[i].position, &sound_channels[i].source->x, &volume, &pan, sound_channels[i].source);
        sound_channels[i].volume = volume;
        if (volume == 0 && i == 3) {
            sound_stop_channel(3);
            continue;
        }
        func_000A20BF(sos_digi_handle, sound_channels[i].handle, pan);
        func_000A1ED5(sos_digi_handle, sound_channels[i].handle, (short)volume << 16 | (short)volume);
    }
    sound_listener_yaw = player_object->yaw;
}
