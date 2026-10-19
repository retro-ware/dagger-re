/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x000684E9 */
#include "records.h"

extern double trade_haggle_exp_rate;
extern double trade_haggle_exp_scale;
extern double trade_haggle_attr_factor;
extern double trade_haggle_skill_factor;
extern double trade_haggle_factor_c;
extern signed char trade_haggle_shown;
extern struct building *current_building;
extern signed char screen_active;
extern signed char game_mode;
extern double trade_haggle_minimum;
extern double trade_haggle_asking;
extern double trade_haggle_discount;
extern int trade_haggle_last_offer;
extern struct character *trade_haggle_buyer;
extern iptr trade_haggle_seller;
extern void trade_haggle_show_offer(void);
extern void trade_haggle_step(void);
extern void mode_push(void);
#pragma aux exp parm routine [] value [8087];
extern double exp(double);

void trade_haggle_start(int price, int quantity, struct record *seller, struct record *buyer)
{
    trade_haggle_last_offer = -1;
    trade_haggle_seller = (iptr)&seller->data.character;
    trade_haggle_buyer = &buyer->data.character;
    if (quantity > 1) {
        /* the original multiplied by a literal (fmul [const]); with the constant as an extern
         * D_ symbol 10.0a would emit fld/fmulp when storing straight to memory, and the
         * (float) conversion (a no-op on the x87 stack) keeps the result in ST(0) instead */
        trade_haggle_discount = (float)(exp(quantity * trade_haggle_exp_rate) * trade_haggle_exp_scale);
    }
    trade_haggle_asking = price * quantity;
    trade_haggle_minimum = ((((1.0 - (trade_haggle_buyer->attributes[5] * trade_haggle_attr_factor)) + (trade_haggle_buyer->reputation[1] * trade_haggle_attr_factor)) - (trade_haggle_buyer->skills[21].value * trade_haggle_skill_factor)) - ((21 - current_building->quality) * trade_haggle_factor_c)) * trade_haggle_asking;
    trade_haggle_step();
    mode_push();
    game_mode = 13;
    screen_active = 1;
    trade_haggle_show_offer();
    trade_haggle_shown = 0;
}
