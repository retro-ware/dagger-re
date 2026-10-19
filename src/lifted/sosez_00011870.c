/* sosez.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "structs.h"
#include "clib.h"

extern char str_sosez_c[];
extern iptr music_song_buffer;
extern int midi_bsa;
extern iptr music_song_data;
extern int D_001A3F4C;

extern int archive_find_record(int, char *, int);
extern int archive_record_size(int, int);
extern iptr archive_read_record(int, int, iptr);
extern int dpmi_lock_region(iptr, int);
#pragma aux sosconv "*" parm caller [] value [eax] modify [eax ebx ecx edx];
#pragma aux (sosconv) sos_load_song;

int sos_load_song(char *name)
{
    struct sos_song *song;
    int song_handle;
    iptr buffer;
    int size;
    int unused;
    int record;
    int record_size;

    record = archive_find_record(midi_bsa, name, 13);
    record_size = archive_record_size(midi_bsa, record);
    size = record_size;
    buffer = (iptr)mc_malloc(size + (int)sizeof(struct sos_song), str_sosez_c, 385);
    if (buffer == 0) return 0;
    archive_read_record(midi_bsa, record, buffer + (int)sizeof(struct sos_song));
    song = (struct sos_song *)buffer;
    mc_memset(song, 0, sizeof(struct sos_song), str_sosez_c, 397, 4);
    song->data = (char *)(buffer + (int)sizeof(struct sos_song));
    if (func_000A021C(song, &song_handle) != 0) {
        if (buffer != 0 && buffer != (-1751672937)) {
            mc_free((void *)buffer, str_sosez_c, 406);
            buffer = -1751672937;
        }
        return -1;
    }
    music_song_buffer = buffer;
    music_song_data = buffer;
    dpmi_lock_region(buffer, (D_001A3F4C = size + (int)sizeof(struct sos_song)));
    return song_handle;
}
