/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00069E3C */
extern char dpmi_mem_info[];
extern int dpmi_mem_free_pages;
extern int dpmi_mem_total_pages;
extern int dpmi_mem_free_unlocked_pages;
extern int mem_free_k;
extern int mem_total_k;
extern int mem_used_k_total;
extern int mem_used_k;
extern void dpmi_get_free_memory(char *);

void dpmi_memory_stats(void)
{
    dpmi_get_free_memory(dpmi_mem_info);
    mem_used_k_total = (dpmi_mem_total_pages - dpmi_mem_free_pages) << 2;
    mem_free_k = (dpmi_mem_free_unlocked_pages << 2) + mem_used_k_total;
    mem_total_k = dpmi_mem_total_pages << 2;
    mem_used_k = mem_used_k_total;
}
