/* camera.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "ptrint.h"
#include "clib.h"
#include "doslow.h"

extern signed char key_down_backslash;
extern char str_camera_c[];
extern char str_pics_scr_bmp[];
extern char bmp_header[];
extern signed char bmp_palette_b[];
extern signed char bmp_palette_g[];
extern signed char bmp_palette_r[];
extern short screenshot_number;
extern char screenshot_name[];

extern void xn_pal_read_dac(char *);
extern int xn_kbd_flush(void);
void screenshot_save_bmp(char *);
#pragma aux mc_set_location parm routine [];

void screenshot_poll(void)
{
    if (key_down_backslash == 0) return;
    mc_set_location(44, str_camera_c);
    mc_sprintf(screenshot_name, str_pics_scr_bmp, (int)(short)screenshot_number);
    screenshot_save_bmp(screenshot_name);
    while (key_down_backslash != 0) xn_kbd_flush();
}

void screenshot_save_bmp(char *filename)
{
    iptr buffer;
    slot16 i;
    slot16 fd;

    buffer = (iptr)mc_malloc(768, str_camera_c, 67);
    xn_pal_read_dac((char *)buffer);
    *(int *)&i = 0;
    for (; ((int)(short)i) < 256; (*(int *)&i)++) {
        bmp_palette_b[((int)(short)i) << 2] = *(signed char *)((char *)((((int)(short)i) * 3) + buffer) + 2) << 2;
        bmp_palette_g[((int)(short)i) << 2] = *(signed char *)((char *)((((int)(short)i) * 3) + buffer) + 1) << 2;
        bmp_palette_r[((int)(short)i) << 2] = *(signed char *)((char *)((((int)(short)i) * 3) + buffer)) << 2;
    }
    screenshot_number++;
    unlink(filename);
    *(int *)&fd = open(filename, 546, 384);
    if (((int)(short)fd) != (-1) && buffer != 0) {
        write((int)(short)fd, bmp_header, 1078);
        if (buffer != 0 && buffer != (-1751672937)) {
            mc_free((void *)buffer, str_camera_c, 82);
            buffer = -1751672937;
        }
        buffer = (iptr)DOS_LOW(0xA0000);
        buffer += 63680;
        *(int *)&i = 0;
        for (; ((int)(short)i) < 200; (*(int *)&i)++) {
            write((int)(short)fd, (void *)buffer, 320);
            buffer += -320;
        }
        close((int)(short)fd);
        return;
    }
    if (buffer == 0 || buffer == (-1751672937)) return;
    mc_free((void *)buffer, str_camera_c, 93);
    buffer = -1751672937;
}
