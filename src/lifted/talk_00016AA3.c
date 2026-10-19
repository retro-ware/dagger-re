/* talk.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "ptrint.h"
#include "clib.h"

extern char str_chld00i0_rci[];
extern char str_faces_cif[];
extern signed char talk_face_loaded;
extern iptr talk_face_image;

extern int disk_open_data(char *);
extern int rand_range(int, int);

void talk_load_face(int face)
{
    int file;
    int index;

    talk_face_loaded = 1;
    if (face >= 1000) {
        index = ((face == 1000) ? 0 : 6);
        index += rand_range(0, 5);
        file = disk_open_data(str_chld00i0_rci);
        lseek(file, index << 12, 0);
    } else {
        talk_face_loaded = 1;
        file = disk_open_data(str_faces_cif);
        lseek(file, face << 12, 0);
    }
    read(file, (void *)talk_face_image, 4096);
    close(file);
}
