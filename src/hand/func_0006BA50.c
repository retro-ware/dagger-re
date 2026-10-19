/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0006BA50 */
#include "records.h"
#include "clib.h"

extern char *screen_buffer;
extern char str_bank_c[];        /* __FILE__ */
extern char str_at[];
extern char D_00190B44[];
extern signed char text_rsc_buffer[];
extern struct image *screen_image_1;
extern struct image *window_image;
extern int game_minutes;
extern struct house_for_sale bank_houses_for_sale[];
extern char *bank_saved_screen;
extern struct ship_for_sale bank_ships_for_sale[];
extern struct bank_account *bank_account;
extern unsigned char bank_screen;
extern unsigned char bank_selected;
extern void parse_expand(unsigned char *, char *);
extern void bank_draw_preview(int, struct block_model *);
extern void bank_draw_house_list(void);
extern void bank_draw_ship_list(void);
extern void text_draw_coloured(char *, short, short, int, unsigned char);
extern int gold_total(void);
extern void xn_draw_image(int, int, int, int, char *);

void bank_draw(void)
{
    int saved_minutes;
    struct image *image;

    image = window_image;
    mc_memcpy(screen_buffer, bank_saved_screen, 64000, str_bank_c, 216, 4);
    xn_draw_image(image->x, image->y, image->width, image->height, image->pixels);
    switch (bank_screen) {
    case 0:
        text_draw_coloured(itoa(bank_account->balance, ((char *)text_rsc_buffer), 10), 197, 19, 145, 156);
        text_draw_coloured(itoa(gold_total(), ((char *)text_rsc_buffer), 10), 203, 29, 145, 156);
        if (bank_account->loan_due != 0) {
            text_draw_coloured(itoa(bank_account->loan_owed, ((char *)text_rsc_buffer), 10), 143, 39, 145, 156);
            saved_minutes = game_minutes;
            game_minutes = bank_account->loan_due;
            parse_expand(str_at, D_00190B44);
            text_draw_coloured(D_00190B44, 119, 49, 145, 156);
            game_minutes = saved_minutes;
        }
        break;
    case 1:
        xn_draw_image(screen_image_1->x, screen_image_1->y, screen_image_1->width, screen_image_1->height, screen_image_1->pixels);
        bank_draw_house_list();
        bank_draw_preview(bank_houses_for_sale[bank_selected].block->model_count, bank_houses_for_sale[bank_selected].block->models);
        break;
    case 2:
        xn_draw_image(screen_image_1->x, screen_image_1->y, screen_image_1->width, screen_image_1->height, screen_image_1->pixels);
        bank_draw_ship_list();
        bank_draw_preview(1, &bank_ships_for_sale[bank_selected].model);
        break;
    }
}
