/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00035D6D */
#include "records.h"
#include "clib.h"

extern char str_fs2df_c[];       /* __FILE__ */
extern char str_rdb[];
extern char str_rdi[];
extern unsigned char str_nwlsbm[];
extern signed char text_buffer[];
extern struct record *location_object;
extern char *scratch_buffer;
extern unsigned char dungeon_block_special;
extern int blocks_bsa;
extern struct record *block_quarters[2][2];
extern char *rdb_data;
extern int rdb_light_level;
extern int rdb_water_level;
extern struct rdb_file *rdb_loaded_file;
extern struct dungeon_block *rdb_dungeon_block;
extern int rdb_object_id_count;
extern int archive_find_record(int, char *, int);
extern iptr archive_read_record(int, int, char *);
extern void rdb_create_objects(struct record *, struct rdb_object *, int);
extern void rdb_link_actions(struct record *, struct rdb_object *, int);
extern struct record *object_create_in_block(struct record *, int, int, int, int);
extern void xn_tex_cache_begin_frame(void);
#pragma aux mc_set_location parm routine [];

void dungeon_load_rdb_block(struct dungeon_block *block)
{
    int i;
    int j;
    int *list_offsets;
    int record;
    struct record *rdi_object;
    struct rdb_object *object_list;

    rdb_dungeon_block = block;
    rdb_data = scratch_buffer;
    mc_set_location(59, str_fs2df_c);
    mc_sprintf(((char *)text_buffer), str_rdb, str_nwlsbm[block->index], block->number);
    record = archive_find_record(blocks_bsa, ((char *)text_buffer), 13);
    archive_read_record(blocks_bsa, record, rdb_data);
    rdb_light_level = 16;
    rdb_water_level = 10000;
    dungeon_block_special = 0;
    rdb_object_id_count = 0;
    rdb_loaded_file = (struct rdb_file *)rdb_data;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            block_quarters[j][i] = object_create_in_block(location_object, 47, 0, 0, i * rdb_loaded_file->width + j);
            block_quarters[j][i]->image = block->number;
            block_quarters[j][i]->x = location_object->x + (j << 10) + (block->x << 11);
            block_quarters[j][i]->z = location_object->z + (i << 10) + (block->z << 11);
        }
    }
    list_offsets = (int *)(rdb_loaded_file->object_root_offset + rdb_data);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (list_offsets[i * rdb_loaded_file->width + j] <= 0)
                continue;
            xn_tex_cache_begin_frame();
            object_list = (struct rdb_object *)(list_offsets[i * rdb_loaded_file->width + j] + rdb_data);
            rdb_create_objects(block_quarters[j][i], object_list, i * rdb_loaded_file->width + j);
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (list_offsets[i * rdb_loaded_file->width + j] <= 0)
                continue;
            object_list = (struct rdb_object *)(list_offsets[i * rdb_loaded_file->width + j] + rdb_data);
            rdb_link_actions(block_quarters[j][i], object_list, i * rdb_loaded_file->width + j);
            block_quarters[j][i]->light_level = rdb_light_level;
            block_quarters[j][i]->water_level = rdb_water_level;
            block_quarters[j][i]->pad13 = dungeon_block_special;
        }
    }
    rdi_object = object_create_in_block(block_quarters[0][0], 60, 512, 0, 0);
    mc_set_location(107, str_fs2df_c);
    mc_sprintf(((char *)text_buffer), str_rdi, str_nwlsbm[block->index], block->number);
    record = archive_find_record(blocks_bsa, ((char *)text_buffer), 13);
    archive_read_record(blocks_bsa, record, RECORD_DATA(rdi_object));
    rdi_object->x = block_quarters[0][0]->x;
    rdi_object->z = block_quarters[0][0]->z;
}
