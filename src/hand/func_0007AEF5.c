/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007AEF5 */
#include <dos.h>
#include "clib.h"
extern char str_loadsave_c[];       /* __FILE__ */
extern char str_save[];
extern char str_atf[];
extern char str_amf[];
extern signed char text_buffer[];
extern char arena2_path[];
extern void disk_copy_file(char *, char *, char *);
extern unsigned func_000A13DA(char *, unsigned, struct find_t *);   /* _dos_findfirst */
extern unsigned func_000A13F7(struct find_t *);                     /* _dos_findnext */
#pragma aux mc_set_location parm routine [];

void save_copy_automap_files(int slot)
{
    int r;
    struct find_t f;

    mc_set_location(787, str_loadsave_c);
    mc_sprintf(((char *)text_buffer), str_atf, arena2_path);
    r = func_000A13DA(((char *)text_buffer), 0, &f);
    mc_set_location(789, str_loadsave_c);
    mc_sprintf(((char *)text_buffer), str_save, slot);
    while (r == 0) {
        disk_copy_file(f.name, arena2_path, ((char *)text_buffer));
        r = func_000A13F7(&f);
    }
    mc_set_location(796, str_loadsave_c);
    mc_sprintf(((char *)text_buffer), str_amf, arena2_path);
    r = func_000A13DA(((char *)text_buffer), 0, &f);
    mc_set_location(798, str_loadsave_c);
    mc_sprintf(((char *)text_buffer), str_save, slot);
    while (r == 0) {
        disk_copy_file(f.name, arena2_path, ((char *)text_buffer));
        r = func_000A13F7(&f);
    }
}
