/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007EED8 */
#include <i86.h>
#include "clib.h"
extern char str_support_c[];
extern signed char swift_present;
extern int swift_call(int, int);

void swift_poll(void)
{
    union REGS regs;
    struct SREGS sregs;
    int unused_1;
    int unused_2;

    if (*((char *)&swift_present) == 0) return;
    mc_memset(&sregs, 0, 12, str_support_c, 1044, 4);
    swift_call(0x5301, 0);
}
