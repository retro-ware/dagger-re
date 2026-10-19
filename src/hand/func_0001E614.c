/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0001E614 */
#include "records.h"
#include "clib.h"
#include "portio.h"
struct bits { unsigned char lo:4; unsigned char b4:1; unsigned char b5:2; };
extern char str_maploads_c[];
extern char str_fmt_s_2[];
extern char str_alms[];
extern char rmb_name_templates[][13];   /* 'TVRN????.RMB'... */
extern signed char text_buffer[];
extern char rmb_template_char;
extern unsigned char location_block_indexes[];
extern unsigned char location_block_numbers[];
extern struct bits location_block_letters[];
extern struct rmb_file *rmb_block;
extern int blocks_bsa;
extern char cfg_block_str[];
extern char cfg_debug;
extern int archive_find_record(int, char *, int);
extern iptr archive_read_record(int, int, iptr);
#pragma aux mc_set_location parm routine [];

void town_block_load_rmb(int block_index)
{
    int template;
    int unused;
    int record;
    char digits[4];

    template = location_block_indexes[block_index];
    if (location_block_letters[block_index].b4)
        rmb_name_templates[template][4] = rmb_template_char;
    else
        rmb_name_templates[template][4] = 'A';
    rmb_name_templates[template][5] = str_alms[location_block_letters[block_index].b5];
    itoa(location_block_numbers[block_index], digits, 10);
    if (template == 13 || template == 14) {
        rmb_name_templates[template][6] = location_block_letters[block_index].lo + 'A';
        rmb_name_templates[template][7] = digits[0];
    } else if (location_block_numbers[block_index] < 10) {
        rmb_name_templates[template][6] = '0';
        rmb_name_templates[template][7] = digits[0];
    } else {
        rmb_name_templates[template][6] = digits[0];
        rmb_name_templates[template][7] = digits[1];
    }
    if (cfg_debug == 0) {
        mc_set_location(476, str_maploads_c);
        mc_sprintf(((char *)text_buffer), str_fmt_s_2, rmb_name_templates[template]);
    } else {
        mc_strncpy(((char *)text_buffer), cfg_block_str, 160, str_maploads_c, 478);
    }
    record = archive_find_record(blocks_bsa, ((char *)text_buffer), 8);
    PORT_READ_RMB(blocks_bsa, record, rmb_block);
}
