/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007B0FC */
#include <dos.h>
#include "clib.h"
extern char *D_00147954;
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
    mc_sprintf(D_00147954, str_atf, arena2_path);
    rc = func_000A13DA(D_00147954, 0, &ff);
    while (rc == 0) {
        mc_set_location(839, str_loadsave_c);
        mc_sprintf(D_00147954, str_fmt_s_s_4, arena2_path, ff.name);
        unlink(D_00147954);
        rc = func_000A13F7(&ff);
    }
    mc_set_location(844, str_loadsave_c);
    mc_sprintf(D_00147954, str_amf, arena2_path);
    rc = func_000A13DA(D_00147954, 0, &ff);
    while (rc == 0) {
        mc_set_location(848, str_loadsave_c);
        mc_sprintf(D_00147954, str_fmt_s_s_4, arena2_path, ff.name);
        unlink(D_00147954);
        rc = func_000A13F7(&ff);
    }
}
