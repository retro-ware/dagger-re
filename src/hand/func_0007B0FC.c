/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007B0FC */
#include <dos.h>
#include "clib.h"
extern char *big_buffer;
extern char str_loadsave_c[];
extern char str_fmt_s_s_4[];
extern char str_atf[];
extern char str_amf[];
extern char arena2_path[];
extern unsigned func_000A13DA(char *, unsigned, struct find_t *);
extern unsigned func_000A13F7(struct find_t *);
#pragma aux mc_set_location parm routine [];

void automap_delete_files(void)
{
    struct find_t ff;
    unsigned rc;

    mc_set_location(835, str_loadsave_c);
    mc_sprintf(big_buffer, str_atf, arena2_path);
    rc = func_000A13DA(big_buffer, 0, &ff);
    while (rc == 0) {
        mc_set_location(839, str_loadsave_c);
        mc_sprintf(big_buffer, str_fmt_s_s_4, arena2_path, ff.name);
        unlink(big_buffer);
        rc = func_000A13F7(&ff);
    }
    mc_set_location(844, str_loadsave_c);
    mc_sprintf(big_buffer, str_amf, arena2_path);
    rc = func_000A13DA(big_buffer, 0, &ff);
    while (rc == 0) {
        mc_set_location(848, str_loadsave_c);
        mc_sprintf(big_buffer, str_fmt_s_s_4, arena2_path, ff.name);
        unlink(big_buffer);
        rc = func_000A13F7(&ff);
    }
}
