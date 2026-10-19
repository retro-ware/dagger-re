/* jmem.c: functions first lifted from the assembly by tools/lift_all.py (2026-10), now
 * ordinary source: edit them here. Keep each function where it is: Watcom aligns switch
 * tables from the start of the file, so moving functions can change the code. */
#include "records.h"
#include "clib.h"

extern signed char key_down_enter;
extern char str_jmem_c[];
extern char str_insufficent_memory_to_service_request[];
extern char str_daggerfall_has_detected_the_error_press[];
extern char str_lgbk00i0_img_2[];
extern signed char world_running;
extern int mem_check_level;
extern int frame_checkpoint;
extern struct record *player_object;
extern iptr window_image;
extern unsigned char game_mode_under_popup;
extern signed char screen_active;
extern signed char game_mode;
extern int logbook_first_entry;
extern char logbook_show_notes[];

extern int key_action_held(int);
extern int sound_play(int, struct record *, int);
extern iptr disk_read_file(char *, iptr);
extern void xn_gfx_restore_mode(void);
extern void fatal_error(char *);
extern void sound_shutdown_music(void);
extern void mem_check_heap(int);
extern void logbook_build_entries(void);
#pragma aux func_0009DA1C parm routine [];
#pragma aux sosconv "*" parm caller [] value [eax] modify [eax ebx ecx edx];
#pragma aux (sosconv) crash_screen;

void mem_pool_init(struct mem_pool *pool, int size)
{
    struct mem_block *block;

    mc_memset(pool, 0, 4, str_jmem_c, 60, 4);
    pool->first = (struct mem_block *)mc_malloc(size, str_jmem_c, 62);
    block = pool->first;
    if (block == 0) fatal_error(str_insufficent_memory_to_service_request);
    pool->size = size;
    block->size = size - MEM_BLOCK_HEADER_SIZE;
    block->prev = 0;
    block->next = block->prev;
    block->flags = 0;
}

void mem_pool_free(struct mem_pool *pool)
{
    if (pool->first == 0 || (iptr)pool->first == (-1751672937)) return;
    mc_free(pool->first, str_jmem_c, 84);
    pool->first = (struct mem_block *)(iptr)-1751672937;
}

iptr mem_pool_alloc(struct mem_pool *pool, int size)
{
    struct mem_block *block;
    struct mem_block *next;
    int block_size;

    block_size = (size + 1) & -2;
    block_size += MEM_BLOCK_HEADER_SIZE;
    block = pool->first;
    while (block != 0) {
        if ((int)block->size < block_size || ((int)(short)((short)block->flags & 1)) != 0) {
            next = block;
            block = block->next;
        } else {
            break;
        }
    }
    if (block == 0) return 0;
    if ((int)block->size != block_size) {
        next = (struct mem_block *)((iptr)block + block_size);
        next->next = block->next;
        next->prev = block;
        next->size = (int)block->size - block_size;
        next->flags = 0;
        next->magic = 1768515945;
        if (next->next != 0) next->next->prev = next;
    } else {
        next = block->next;
        block_size += MEM_BLOCK_HEADER_SIZE;
    }
    block->next = next;
    block->size = block_size - MEM_BLOCK_HEADER_SIZE;
    block->flags |= 1;
    block->magic = 1768515945;
    return (iptr)block + MEM_BLOCK_HEADER_SIZE;
}

int mem_block_size(char *data)
{
    struct mem_block *block;

    block = (struct mem_block *)(data - MEM_BLOCK_HEADER_SIZE);
    return block->size;
}

void crash_screen(void)
{
    xn_gfx_restore_mode();
    func_0009DA1C(394, str_jmem_c);
    printf(str_daggerfall_has_detected_the_error_press, frame_checkpoint);
    while (key_down_enter == 0);
    sound_shutdown_music();
    func_000A2D9E();
}

void mem_check_now(int checkpoint)
{
    mem_check_level++;
    mem_check_heap(checkpoint);
    mem_check_level--;
}

int logbook_open(int force)
{
    if (((int)game_mode_under_popup) == 14 && ((int)(unsigned char)game_mode) == 8) {
        return 1;
    }
    if (force != 0 || (game_mode == 0 && key_action_held(24) != 0)) {
        game_mode = 14;
        screen_active = 1;
        window_image = disk_read_file(str_lgbk00i0_img_2, 0);
        *(int *)logbook_show_notes = (logbook_first_entry = 0);
        world_running = 0;
        logbook_build_entries();
        sound_play(237, player_object, 100);
    }
    return ((((int)(unsigned char)game_mode) == 14) ? 1 : 0);
}
