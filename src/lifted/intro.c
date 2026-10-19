/* intro.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "ptrint.h"
#include "clib.h"
#include "doslow.h"

extern signed char mouse_buttons;
extern iptr screen_buffer;
extern char str_anim0001_vid[];
extern char str_intro_c[];
extern char str_anim0000_vid[];
extern char str_anim0011_vid[];
extern char str_dag2_vid[];

extern iptr disk_resolve_path(iptr);
extern int xn_vid_play(char *, int, int, int);
extern int xn_mouse_poll_clamped(void);
extern void starting_equipment_give(void);
extern void palette_restore(void);

void intro_play_logo(void)
{
    int unused1;
    int unused2;
    int unused3;
    int unused4;
    int unused5;
    int unused6;
    int unused7;
    int unused8;
    int unused9;
    int unused10;
    int unused11;
    int unused12;
    iptr path;

    while (mouse_buttons != 0) xn_mouse_poll_clamped();
    path = disk_resolve_path((iptr)str_anim0001_vid);
    xn_vid_play((char *)path, 0, 0, 1);
    mc_memset((void *)DOS_LOW(0xA0000), 0, 64000, str_intro_c, 34, 4);
    mc_memset((void *)screen_buffer, 0, 64000, str_intro_c, 35, 4);
    while (mouse_buttons != 0) xn_mouse_poll_clamped();
    mc_memset((void *)DOS_LOW(0xA0000), 0, 64000, str_intro_c, 39, 4);
    palette_restore();
}

void intro_play_movie(void)
{
    iptr path;

    mc_memset((void *)DOS_LOW(0xA0000), 0, 64000, str_intro_c, 358, 4);
    while (mouse_buttons != 0) xn_mouse_poll_clamped();
    path = disk_resolve_path((iptr)str_anim0000_vid);
    xn_vid_play((char *)path, 0, 0, 1);
    mc_memset((void *)DOS_LOW(0xA0000), 0, 64000, str_intro_c, 363, 4);
    mc_memset((void *)screen_buffer, 0, 64000, str_intro_c, 364, 4);
    while (mouse_buttons != 0) xn_mouse_poll_clamped();
    path = disk_resolve_path((iptr)str_anim0011_vid);
    xn_vid_play((char *)path, 0, 0, 1);
    mc_memset((void *)DOS_LOW(0xA0000), 0, 64000, str_intro_c, 369, 4);
    mc_memset((void *)screen_buffer, 0, 64000, str_intro_c, 370, 4);
    while (mouse_buttons != 0) xn_mouse_poll_clamped();
    path = disk_resolve_path((iptr)str_dag2_vid);
    xn_vid_play((char *)path, 32, 0, 1);
    mc_memset((void *)DOS_LOW(0xA0000), 0, 64000, str_intro_c, 375, 4);
    mc_memset((void *)screen_buffer, 0, 64000, str_intro_c, 376, 4);
    palette_restore();
}

void chargen_give_starting_equipment(void)
{
    starting_equipment_give();
}
