/* click.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "ptrint.h"
#include "clib.h"

extern char str_click_c[];
extern char str_books_bok_txt_4[];
extern signed char text_buffer[];

extern int disk_open_data(char *);
#pragma aux mc_set_location parm routine [];

void book_read_header(char *header, int book_id)
{
    int handle;

    mc_set_location(586, str_click_c);
    mc_sprintf((char *)text_buffer, str_books_bok_txt_4, book_id);
    handle = disk_open_data(text_buffer);
    read(handle, header, 234);
    close(handle);
}
