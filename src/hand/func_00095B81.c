/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x00095B81 */
#include "records.h"

extern int D_001940D6;
extern int ui_dirty_flags;
extern struct record *wagon_container;
extern struct record *player_object;
extern struct character *player_character;
extern int picked_model_index;
extern struct record *inv_right_rows[];
extern int inv_right_scroll;
extern short inv_right_item_count;
extern void inv_draw_item_cell(struct record *, short, iptr);
extern int xn_math_approx_dist2d(int, int, int, int);
extern int xn_math_approx_hypot(int, int);
extern iptr xn_str_find_u32(char *, struct record *, int);

void inv_list_right_item(struct record *object, iptr rects)
{
    struct item *item;

    if (object->type != 2 && object->type != 54 || (object->flags & 2))
        return;
    if (object->flags & 0x200)
        return;
    if ((D_001940D6 & 4) && object->owner != picked_model_index)
        return;
    if ((!(D_001940D6 & 4) && object->parent != wagon_container ? 1 : 0) && xn_math_approx_hypot(object->z - player_object->z, xn_math_approx_dist2d(object->x, object->y, player_object->x, player_object->y)) > 160)
        return;
    if (inv_right_item_count >= inv_right_scroll && inv_right_item_count < inv_right_scroll + 4) {
        if (!(ui_dirty_flags & 4) && PTR_TABLE_FIND_ARGS((char *)player_character->equipped, object, 27) != 0) {
            item = &object->data.item;
            if (item->group != 1)
                return;
        }
        inv_right_rows[inv_right_item_count - inv_right_scroll] = object;
        inv_draw_item_cell(object, inv_right_item_count - inv_right_scroll, rects);
    }
    inv_right_item_count++;
}
