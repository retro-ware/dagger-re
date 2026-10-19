/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0007EF86 */
#include "ptrint.h"
#include <i86.h>
#include "clib.h"
extern char str_support_c[];
extern char swift_info[];
extern char swift_rm_regs[];
extern int swift_rm_regs_ebx;
extern int swift_rm_regs_edx;
extern int swift_rm_regs_eax;
extern short swift_rm_regs_es;
extern short swift_dos_segment;
extern short swift_dos_selector_seg;
extern int func_000A2EC5(void __far *, void __far *, unsigned, char *, int, int);

int swift_call(int ax_value, int bx_value)
{
    union REGS regs;
    struct SREGS sregs;

    mc_memset(&sregs, 0, 12, str_support_c, 1078, 4);
    mc_memset(&regs, 0, 28, str_support_c, 1079, 4);
    mc_memset(swift_rm_regs, 0, 50, str_support_c, 1080, 4);
    swift_rm_regs_eax = ax_value;
    swift_rm_regs_ebx = bx_value;
    swift_rm_regs_es = swift_dos_segment;
    swift_rm_regs_edx = 0;
    regs.w.ax = 0x300;
    regs.w.bx = 0x33;
    sregs.es = FP_SEG(swift_rm_regs);
    regs.x.edi = FP_OFF(swift_rm_regs);
    int386x(0x31, &regs, &regs, &sregs);
    func_000A2EC5(swift_info, MK_FP((unsigned short)swift_dos_selector_seg, 0), 14, str_support_c, 1094, 4);
    return swift_rm_regs_eax;
}
