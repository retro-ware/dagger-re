/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0006D6A4 */
#include <dos.h>
#include "clib.h"
extern char str_disk_c[];        /* __FILE__ */
extern char str_fmt_s_6[];
extern char str_fmt_s_7[];
extern char str_dot[];
extern char str_dotdot[];
extern char str_backslash_star_dot_star[];
extern void file_index_add_dir(char *);
extern void file_index_add_name(char *);
#pragma aux mc_set_location parm routine [];
extern int func_000A13DA(char *, int, struct find_t *);
extern int func_000A13F7(struct find_t *);

void file_index_scan(char *dir)
{
    int rc;
    int i;
    char path[80];
    struct find_t ff;

    if (dir[strlen(dir) - 1] != '\\') {
        mc_set_location(337, str_disk_c);
        mc_sprintf(path, str_fmt_s_6, dir);
    } else {
        mc_set_location(339, str_disk_c);
        mc_sprintf(path, str_fmt_s_7, dir);
    }
    rc = func_000A13DA(path, 16, &ff);
    while (rc == 0) {
        if ((ff.attrib & 16) == 0)
            file_index_add_name(ff.name);
        rc = func_000A13F7(&ff);
    }
    rc = func_000A13DA(path, 16, &ff);
    while (rc == 0) {
        if (ff.attrib & 16) {
            if (stricmp(ff.name, str_dot) == 0 || stricmp(ff.name, str_dotdot) == 0) {
                rc = func_000A13F7(&ff);
                continue;
            }
            file_index_add_dir(ff.name);
            mc_strncpy(&path[strlen(path) - 3], ff.name, 4, str_disk_c, 359);
            file_index_scan(path);
            i = strlen(path) - 1;
            while (i != 0 && path[i] != '\\')
                i--;
            mc_strncpy(path + i, str_backslash_star_dot_star, 4, str_disk_c, 363);
        }
        rc = func_000A13F7(&ff);
    }
}
