/* matched by the real Watcom C32 10.0a (-d2): a run of jmem.c from 0x0006A0D4 to 0x0006A54B, kept together for its switch table's alignment */
#include "records.h"
#include "clib.h"

struct msg { int a; short b; char pad[10]; };
extern char *screen_buffer;
extern char str_jmem_c[];       /* __FILE__ */
extern char str_invalid_memory_pointer[];
extern char str_unable_to_locate_quest_object[];
extern char str_duplicate_quest_map_id[];
extern char str_invalid_quest_id_3[];
extern char str_invalid_video_buffer[];
extern char str_last_object_type_was[];
extern char str_invalid_memory_header[];
extern char str_inconsistent_size_field[];
extern char str_invalid_memory_size_field[];
extern char str_invalid_memory_queue_pointer[];
extern char str_invalid_memory_stack_pointer[];
extern char str_invalid_memory_heap[];
extern int mem_check_level;
extern int frame_checkpoint;
extern int engine_running;
extern struct record *nonworld_root;
extern struct record *quest_root;
extern struct record *location_object;
extern struct mem_pool object_heap;
extern int object_heap_size;
extern void debug_checkpoint(int);
extern struct qbn_place *quest_section(struct quest *, int);
extern void fatal_error(char *);
extern void object_foreach(struct record *, void (*)());
extern struct record *object_find_by_id(struct record *, iptr);
extern int func_000A2A76(struct msg *);
extern void xn_sys_zero_page_check(int);
#pragma aux mc_set_location parm routine [];
#pragma aux func_000A29BA parm routine [];

int mem_pool_release(char *data)
{
    struct mem_block *block;
    struct mem_block *neighbour;
    int size;

    block = (struct mem_block *)(data - MEM_BLOCK_HEADER_SIZE);
    if (block->magic != 1768515945 || ((short)block->flags & ~1) != 0)
        fatal_error(str_invalid_memory_pointer);
    *(unsigned char *)&block->flags &= 254;
    size = block->size;
    mc_memset(data, 150, size, str_jmem_c, 182, 4);
    if (block->next != 0) {
        if (!((short)block->next->flags & 1)) {
            neighbour = block->next;
            block->size += neighbour->size + MEM_BLOCK_HEADER_SIZE;
            block->next = neighbour->next;
            if (block->next != 0)
                block->next->prev = block;
        }
    }
    if (block->prev != 0) {
        if (!((short)block->prev->flags & 1)) {
            neighbour = block->prev;
            neighbour->size += block->size + MEM_BLOCK_HEADER_SIZE;
            neighbour->next = block->next;
            if (neighbour->next != 0)
                neighbour->next->prev = neighbour;
        }
    }
    return size;
}

void mem_check_quest_object_cb(struct record *object)
{
    int id;

    if (object->twin != 0 && object->type != 2) {
        if (object_find_by_id(location_object, object->twin->id) == 0)
            fatal_error(str_unable_to_locate_quest_object);
    }
    id = object->id;
    object->id = 0;
    if (object_find_by_id(nonworld_root, id) != 0)
        fatal_error(str_duplicate_quest_map_id);
    object->id = id;
}

void mem_check_quest_ids_cb(struct record *object)
{
    struct qbn_place *place;
    struct quest *quest;
    int i;

    if (object->type != 14)
        return;
    quest = &object->data.quest;
    place = quest_section(quest, 4);
    for (i = 0; quest->section_counts[4] > i; i++, place++) {
        if (place->object != 0 && object->quest_id != place->object->quest_id)
            fatal_error(str_invalid_quest_id_3);
    }
}

void mem_check_heap(int checkpoint)
{
    struct mem_block *block;
    struct mem_block *prev;

    frame_checkpoint = checkpoint;
    if (mem_check_level == 0)
        return;
    mc_set_location(280, str_jmem_c);
    if (func_000A29BA(screen_buffer) != 0)
        fatal_error(str_invalid_video_buffer);
    debug_checkpoint(checkpoint);
    if (engine_running != 0)
        xn_sys_zero_page_check(checkpoint);
    object_foreach(nonworld_root, mem_check_quest_object_cb);
    object_foreach(quest_root->children, mem_check_quest_ids_cb);
    prev = block = object_heap.first;
    while (block != 0) {
        if (block->magic != 1768515945) {
            mc_set_location(300, str_jmem_c);
            func_000A148C(str_last_object_type_was, ((unsigned char *)prev)[MEM_BLOCK_HEADER_SIZE]);
            fatal_error(str_invalid_memory_header);
        }
        if (block->next != 0 && (char *)block + MEM_BLOCK_HEADER_SIZE + block->size != (char *)block->next) {
            mc_set_location(306, str_jmem_c);
            func_000A148C(str_last_object_type_was, ((unsigned char *)block)[MEM_BLOCK_HEADER_SIZE]);
            fatal_error(str_inconsistent_size_field);
        }
        if (block->size == 0 || (int)block->size > object_heap_size) {
            mc_set_location(312, str_jmem_c);
            func_000A148C(str_last_object_type_was, ((unsigned char *)prev)[MEM_BLOCK_HEADER_SIZE]);
            fatal_error(str_invalid_memory_size_field);
        }
        if (block < object_heap.first || (iptr)object_heap.first + object_heap_size < (iptr)block) {
            mc_set_location(318, str_jmem_c);
            func_000A148C(str_last_object_type_was, ((unsigned char *)prev)[MEM_BLOCK_HEADER_SIZE]);
            fatal_error(str_invalid_memory_queue_pointer);
        }
        prev = block;
        block = block->next;
        if (block != 0 && block->prev != prev) {
            mc_set_location(327, str_jmem_c);
            func_000A148C(str_last_object_type_was, ((unsigned char *)prev)[MEM_BLOCK_HEADER_SIZE]);
            fatal_error(str_invalid_memory_stack_pointer);
        }
    }
}

void mem_check_crt_heap(int checkpoint)
{
    int status;
    struct msg info;

    status = 0;
    if (mem_check_level == 0)
        return;
    mc_set_location(349, str_jmem_c);
    func_000A2A2B();
    info.b = 0;
    info.a = 0;
    while (status == 0)
        status = func_000A2A76(&info);
    switch (status) {
    case 4:
        break;
    case 1:
        break;
    case 2:
        fatal_error(str_invalid_memory_heap);
        break;
    case 5:
        fatal_error(str_invalid_memory_heap);
        break;
    case 3:
        fatal_error(str_invalid_memory_heap);
        break;
    }
}
