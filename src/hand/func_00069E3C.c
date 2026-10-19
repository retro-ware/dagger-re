/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00069E3C */
extern char dpmi_mem_info[];
extern int D_001A3F74;
extern int D_001A3F78;
extern int D_001A3F7C;
extern int D_001A3F90;
extern int D_001A3F94;
extern int D_001A3F98;
extern int mem_used_k;
extern void dpmi_get_free_memory(char *);

void dpmi_memory_stats(void)
{
    dpmi_get_free_memory(dpmi_mem_info);
    D_001A3F98 = (D_001A3F78 - D_001A3F74) << 2;
    D_001A3F90 = (D_001A3F7C << 2) + D_001A3F98;
    D_001A3F94 = D_001A3F78 << 2;
    mem_used_k = D_001A3F98;
}
