/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000369A0 */
#include "records.h"
#include "clib.h"

extern char str_fs2df_c[];

void rdb_model_id_from_name(struct record *object, char *name)
{
    int x;
    char buf[12];

    func_000A14E8(buf, name, 3, str_fs2df_c, 347, 9);
    buf[3] = 0;
    object->image2 = atoi(buf);
    func_000A14E8(buf, name + 3, 2, str_fs2df_c, 350, 9);
    buf[2] = 0;
    object->image = atoi(buf);
    func_000A14E8(buf, name, 8, str_fs2df_c, 354, 9);
    buf[8] = 0;
}
