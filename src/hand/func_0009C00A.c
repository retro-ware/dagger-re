/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0009C00A */
#include "records.h"
#include "clib.h"

#pragma pack(1)
struct Q { char pad[4]; char name[28]; };
extern char text_colour;
extern char str_travel_c[];
extern unsigned short travel_options;
extern short travel_trip_option_marks[];
extern int travel_selected_location;
extern signed char text_buffer[];
extern struct character *player_character;
extern struct Q *region_location_names;
extern int travel_trip_minutes;
extern struct image *travel_popup_image;
extern unsigned char travel_map_mode;
extern void text_draw_coloured(char *, short, short, int, unsigned char);
extern void text_draw_centred_coloured(char *, short, short, int, unsigned char);
extern int travel_trip_cost(void);
extern void xn_draw_fill_rect(short, short, short, short);
extern void xn_draw_image(int, int, int, int, char *);

void travel_draw_trip_popup(void)
{
    int i;

    xn_draw_image(travel_popup_image->x, travel_popup_image->y, travel_popup_image->width, travel_popup_image->height, travel_popup_image->pixels);
    if (travel_map_mode == 100) {
        mc_strncpy(((char *)text_buffer), region_location_names[travel_selected_location].name, 160, str_travel_c, 654);
        text_draw_centred_coloured(((char *)text_buffer), 160, 74, 145, 156);
        return;
    }
    text_colour = 199;
    for (i = 0; i < 6; i++) {
        if (travel_options & (1 << i))
            xn_draw_fill_rect(travel_trip_option_marks[i * 4], travel_trip_option_marks[i * 4 + 1], 4, 4);
    }
    text_draw_coloured(itoa(player_character->gold, ((char *)text_buffer), 10), 148, 97, 145, 156);
    text_draw_coloured(utoa(travel_trip_cost(), ((char *)text_buffer), 10), 117, 107, 146, 156);
    text_draw_coloured(itoa(travel_trip_minutes / 1440 + 1, ((char *)text_buffer), 10), 129, 117, 145, 156);
    mc_strncpy(((char *)text_buffer), region_location_names[travel_selected_location].name, 160, str_travel_c, 668);
    text_draw_centred_coloured(((char *)text_buffer), 160, 2, 145, 156);
}
