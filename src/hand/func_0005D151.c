/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0005D151 */
#include "records.h"

struct flags { unsigned char b0:3; unsigned char b3:1; unsigned char b4:1; };
extern struct bss_image *hud_compass_image;
extern struct bss_image *compass_frames_1;
extern struct bss_image *compass_frames_2;
extern struct image *compass_frame_image;
extern struct flags D_001940D6;
extern struct record *camera_object;
extern int int_identity(int);
extern int detect_arrow_anim_frame(void);
extern int icon_cycle_anim_frame(void);
extern void xn_draw_image_transparent(int, int, int, int, char *);

void hud_draw_compass(void)
{
    char *data;
    struct image *u;
    int frame;

    frame = int_identity((camera_object->yaw & 0x7ff) >> 6);
    data = hud_compass_image->pixels + hud_compass_image->width * hud_compass_image->height * frame;
    xn_draw_image_transparent(hud_compass_image->x, hud_compass_image->y, hud_compass_image->width, hud_compass_image->height, data);
    if (D_001940D6.b3) {
        frame = icon_cycle_anim_frame();
        data = compass_frames_1->pixels + frame * (compass_frames_1->width * compass_frames_1->height);
        xn_draw_image_transparent(compass_frames_1->x, compass_frames_1->y, compass_frames_1->width, compass_frames_1->height, data);
    }
    if (D_001940D6.b4) {
        frame = detect_arrow_anim_frame();
        data = compass_frames_2->pixels + frame * (compass_frames_2->width * compass_frames_2->height);
        xn_draw_image_transparent(compass_frames_2->x, compass_frames_2->y, compass_frames_2->width, compass_frames_2->height, data);
    }
    u = compass_frame_image;
    xn_draw_image_transparent(u->x, u->y, u->width, u->height, u->pixels);
}
