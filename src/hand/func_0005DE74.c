/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0005DE74 */
#include "records.h"
#include "clib.h"

extern char str_equip_c[];
extern char str_empty_3[];
extern char str_invalid_index_requested[];
extern struct item_template item_templates[];
extern unsigned char item_init_count;
extern char fatal_error_text[];
extern struct character *player_character;
extern short item_draw_order_scratch;
extern unsigned char D_0019626D;
extern unsigned char item_template_capacity;
extern void fatal_error(char *);
extern void item_set_race_image(struct item *, int);
extern void func_0005E636(struct item *);
extern void item_roll_material(struct item *);
extern void item_roll_armor_type(struct item *);
extern void item_init_book(struct item *, int);
extern void item_make_magic(struct item *, int);
extern int rand_range(int, int);
#pragma aux mc_set_location parm routine [];
#pragma aux func_000A18C3 parm routine [];

#define PFLAGS (player_character->flags)

void item_init_from_template(unsigned short template_id, short group, short index, struct item *item)
{
    struct item_template *template;
    unsigned short requested_id;

    requested_id = template_id;
    if (group == 13) {
        index = 0;
        template_id = 284;
    }
    if (group == 6 && ((unsigned short)PFLAGS & 1) != 0)
        group = 12;
    if (group == 12 && ((unsigned short)PFLAGS & 1) == 0)
        group = 6;
    if (group == 4) {
        item_make_magic(item, -1);
        return;
    }
    if (group == 9 && (index < 2 || index == 4))
        index = 2;
    if (group == 10 && index == 11)
        index = 10;
    if (group == 7) {
        item_init_count++;
        template_id = 277;
        if (index > 3)
            index = rand_range(0, 3);
    }
    if (template_id >= 288) {
        mc_set_location(58, str_equip_c);
        func_000A18C3(str_empty_3);
        mc_set_location(59, str_equip_c);
        mc_sprintf(fatal_error_text, str_invalid_index_requested, requested_id, template_id);
        fatal_error(fatal_error_text);
    }
    template = &item_templates[template_id];
    if (template->inventory_image == 32512)
        item->index = 0;
    mc_strncpy(item->name, template->name, 32, str_equip_c, 68);
    item->group = group;
    item->index = index;
    item->value = template->value;
    if (template->capacity != 0 && (template->flags & 1) != 0) {
        item_template_capacity = template->capacity;
        *(short *)item->pad28 = 0;
    } else {
        item_template_capacity = 0;
        *(short *)item->pad28 = template->capacity;
    }
    item->item_flags = (unsigned short)template->flags;
    item->condition = item->max_condition = template->condition;
    item->magicka_bonus = 0;
    if (template->inventory_image != 0 && template->dropped_image == 0)
        item->dropped_image = template->inventory_image;
    if (template->dropped_image != 0 && template->inventory_image == 0)
        item->inventory_image = template->dropped_image;
    if (template->inventory_image != 0)
        item->inventory_image = template->inventory_image;
    if (template->dropped_image != 0)
        item->dropped_image = template->dropped_image;
    if (((unsigned short)item->inventory_image & -128) == 31360 && ((unsigned short)PFLAGS & 1) == 0) {
        item->inventory_image &= 127;
        item->inventory_image |= 31872;
    }
    item->material = item->armor_type = 0;
    if (group == 1 && (index == 4 || index == 5)) {
        if ((rand() & 3) != 0) {
            if (index == 4)
                item->color = (rand() & 1) + 24;
            else
                item->color = (rand() & 1) + 26;
        }
    } else {
        item->color = 18;
    }
    item->weight = template->weight;
    item->enchant_points = template->enchant_points;
    item->variants = template->variants;
    item->draw_order = template->draw_order;
    mc_memset(item->enchantments, -1, 40, str_equip_c, 118, 40);
    D_0019626D = template->rarity;
    item_draw_order_scratch = template->draw_order;
    if (group == 27 && index == 4)
        item->stack_count = rand() % 20;
    if (group == 6 || group == 12 || group == 2) {
        func_0005E636(item);
        item_set_race_image(item, player_character->race);
    }
    if (group == 3)
        item_roll_material(item);
    if (group == 2) {
        item_roll_armor_type(item);
        if (item->index != 5 && item->index < 7 && item->material == 2)
            item_roll_material(item);
    }
    if (group == 3 && index == 18) {
        item->stack_count = rand_range(1, 20);
        item->condition = 0;
    }
    if (group == 7)
        item_init_book(item, index);
    if (group == 13)
        item->message = rand();
}
