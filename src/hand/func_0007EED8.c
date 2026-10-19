/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007EED8 */
#include <i86.h>
#include "clib.h"
extern char str_support_c[];
extern signed char D_00196281;
extern int func_0007EF86(int, int);

void func_0007EED8(void)
{
    union REGS regs;
    struct SREGS sregs;
    int unused_1;
    int unused_2;

    if (*((char *)&D_00196281) == 0) return;
    mc_memset(&sregs, 0, 12, str_support_c, 1044, 4);
    func_0007EF86(0x5301, 0);
}
