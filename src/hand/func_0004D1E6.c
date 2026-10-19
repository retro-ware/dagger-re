/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0004D1E6 */
#include "records.h"
#include "clib.h"

extern char str_note_c[];
extern char str_notebook_tde[];
extern char str_unable_to_create_new_notebook[];
extern char str_mask02i0_img[];
extern char str_note_rci[];
extern unsigned char D_001940D5;
extern struct record *player_object;
extern iptr window_image;
extern char *scratch_buffer;
extern unsigned char game_mode_under_popup;
extern unsigned char screen_active;
extern unsigned char game_mode;
extern int note_file_size;
extern iptr note_rci;
extern iptr note_search_text;
extern iptr note_search_from;
extern char *note_page;
extern char *note_page_backup;
extern short note_page_index;
extern short note_file;
extern unsigned char note_tool;
extern unsigned char note_silent;
extern int key_action_held(int);
extern void fatal_error(char *);
extern int sound_play(int, struct record *, int);
extern iptr disk_read_file(char *, iptr);
extern int disk_open_rw(char *);
extern int disk_create(char *);

int note_open_notebook(short mode)
{
    if (game_mode_under_popup == 9 && game_mode == 8) return 1;
    note_silent = (mode == 100);
    if (mode != 0 || (game_mode == 0 && key_action_held(25) != 0)) {
        D_001940D5 |= 8;
        note_search_text = 0;
        note_search_from = 0;
        note_page = mc_malloc(3640, str_note_c, 67);
        note_page_backup = mc_malloc(3640, str_note_c, 68);
        if ((note_file = disk_open_rw(str_notebook_tde)) < 1) {
            mc_memset(scratch_buffer, 0, 3640, str_note_c, 72, 4);
            if ((note_file = disk_create(str_notebook_tde)) < 1)
                fatal_error(str_unable_to_create_new_notebook);
            write(note_file, scratch_buffer, 3640);
            lseek(note_file, 0, 0);
        }
        read(note_file, note_page, 3640);
        mc_memcpy(note_page_backup, note_page, 3640, str_note_c, 79, 4);
        note_tool = 0;
        note_page_index = note_tool;
        note_file_size = filelength(note_file);
        if (note_silent == 0) {
            game_mode = 9;
            window_image = disk_read_file(str_mask02i0_img, 0);
            note_rci = disk_read_file(str_note_rci, 0);
            screen_active = 1;
            sound_play(237, player_object, 100);
        }
    }
    return game_mode == 9;
}
