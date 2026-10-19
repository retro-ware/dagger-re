/* crime.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "ptrint.h"
#include "clib.h"
#include "doslow.h"

extern iptr screen_buffer;
extern char str_crime_c[];
extern char str_pris00i0_img[];
extern char str_days_until_freedom[];
extern signed char text_buffer[];
extern int sky_loaded_frame;
extern signed char time_skip_active;
extern signed char night_sky_loaded;
extern signed char time_skip_quiet;
extern signed char politics_new_year;
extern signed char reputation_decay_off;

extern iptr disk_read_file(char *, iptr);
extern void xn_pal_set_range_8bit(char *, int, int);
extern void xn_gfx_present_inclusive(int);
extern void screen_shake_stop(void);
extern void court_restore_vitals(void);
extern void time_pass(int);
extern void palette_restore(void);
extern void text_draw_centred_coloured(iptr, int, int, int, unsigned char);
#pragma aux mc_set_location parm routine [];

void prison_serve_sentence(int days)
{
    int day;
    iptr image;

    reputation_decay_off = 1;
    time_skip_quiet = 1;
    politics_new_year = 0;
    time_skip_active = 1;
    image = disk_read_file(str_pris00i0_img, 0);
    mc_memset((void *)DOS_LOW(0xA0000), 0, 64000, str_crime_c, 384, 4);
    for (day = 0; day < 768; day++) {
        *(signed char *)((char *)(image + day) + 64000) <<= 2;
    }
    xn_pal_set_range_8bit((char *)(image + 64000), 0, 256);
    for (day = days; day != 0; day--) {
        time_pass(1440);
        mc_memcpy((void *)screen_buffer, (void *)image, 64000, str_crime_c, 391, 4);
        mc_set_location(392, str_crime_c);
        mc_sprintf((char *)text_buffer, str_days_until_freedom, day);
        text_draw_centred_coloured((iptr)text_buffer, 156, 165, 190, 219);
        xn_gfx_present_inclusive(0);
    }
    mc_memset((void *)DOS_LOW(0xA0000), 0, 64000, str_crime_c, 397, 4);
    mc_memset((void *)screen_buffer, 0, 64000, str_crime_c, 398, 4);
    palette_restore();
    if (image != 0 && image != (-1751672937)) {
        mc_free((void *)image, str_crime_c, 400);
        image = -1751672937;
    }
    time_skip_active = 0;
    politics_new_year = 1;
    reputation_decay_off = 0;
    sky_loaded_frame = 10000;
    night_sky_loaded = 0;
    time_skip_quiet = 0;
    screen_shake_stop();
    court_restore_vitals();
}
