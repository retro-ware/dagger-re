/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0005F81F */
#include <dos.h>
#include "clib.h"
#include "doslow.h"
struct savehdr { char pad[128]; char f128; char pad81[99]; short f228; };
struct slot { short id; short f2; };
struct flags16 { unsigned short f0; };
extern char str_equip_c[];        /* __FILE__ */
extern char str_bok_txt[];
extern char str_books_2[];
extern char str_books_3[];
extern struct slot book_list[];
extern signed char text_buffer[];
extern struct flags16 *game_settings;
extern struct savehdr *scratch_buffer;
extern char *books_path;
extern unsigned short book_count;
extern int disk_open_data(char *);
extern int func_000A13DA(char *, int, struct find_t *);
extern int func_000A13F7(struct find_t *);
#pragma aux mc_set_location parm routine [];

void books_scan(void)
{
    int fd;
    int rc;
    struct find_t ff;
    struct savehdr *buf;

    book_count = 0;
    buf = scratch_buffer;
    mc_set_location(694, str_equip_c);
    mc_sprintf(((char *)text_buffer), str_books_2, books_path, str_bok_txt);
    rc = func_000A13DA(((char *)text_buffer), 0, &ff);
    while (rc == 0) {
        book_list[book_count].id = atoi(ff.name + 3);
        mc_set_location(699, str_equip_c);
        mc_sprintf(((char *)text_buffer), str_books_3, ff.name);
        fd = disk_open_data(((char *)text_buffer));
        read(fd, buf, 234);
        close(fd);
        if (buf->f128 == 0 || (DOS_NULL(game_settings)->f0 & 4) == 0   /* not set yet: 0 */)
            book_list[book_count++].f2 = buf->f228 - 1;
        rc = func_000A13F7(&ff);
    }
}
