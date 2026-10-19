/* matched by the real Watcom C32 10.0a (-d2), lifted from 0x0004ED36 */
#include "records.h"
#include "clib.h"
#include "doslow.h"

#pragma pack(1)
extern unsigned disk_last_file_size;
extern char *xn_shade_translucent_table;
extern char *xn_water_tint_table;
extern char joystick_status;
extern char xn_joy_calibration[];
extern int cfg_helmet;
extern int xn_helmet_smoothing;
extern char str_init_c[];
extern char str_buttons_rci[];
extern char str_icon00i0_img[];
extern char str_spells_std_2[];
extern char str_main00i0_img[];
extern char str_main03i0_img[];
extern char str_main04i0_img[];
extern char str_main05i0_img[];
extern char str_mcol00i0_cif[];
extern char str_rumor_dat_3[];
extern char str_cmpa00i0_bss[];
extern char str_cmpa01i0_bss[];
extern char str_cmpa02i0_bss[];
extern char str_cmpa03i0_img[];
extern char str_main01i0_img[];
extern char str_face00i0_cif[];
extern char str_face01i0_cif[];
extern char str_face10i0_cif[];
extern char str_face11i0_cif[];
extern char str_kids00i0_cif[];
extern char str_mred00i0_cfa[];
extern char str_mred01i0_cfa[];
extern char str_compbox_img[];
extern char str_compass_img[];
extern char str_count_dat[];
extern char str_arch3d_bsa[];
extern char str_maps_bsa[];
extern char str_dagger_snd[];
extern char str_monster_bsa[];
extern char str_test_dat[];
extern char str_water_tbl[];
extern char str_haze_000[];
extern char str_haze_001[];
extern char str_dagger_grd_2[];
extern char str_unable_to_initialize_the_sound_drivers[];
extern char *controls_file;
extern char default_key_map[];
extern int cfg_texture_memory;
extern struct region regions[];
extern signed char text_buffer[];
extern char *hud_compass_image;
extern char *compass_frames_1;
extern char *compass_frames_2;
extern char *compass_frame_image;
extern char arena2_path[];
extern unsigned char ui_dirty_flags;
extern char *horse_overlay_image;
extern char *cart_overlay_image;
extern int last_skill_check_minutes;
extern int monster_bsa_handle;
extern struct spell *spell_records;
extern char *paperdoll_buffer;
extern char *hud_bar_image;
extern char *hud_mode_icons;
extern char *paperdoll_mask;
extern char *hud_portrait;
extern char *icon_image;
extern char *buttons_rci;
extern int game_minutes;
extern unsigned char *game_settings;
extern int realtime_clock_tick;
extern char *hud_vital_bar_images;
extern char *main_image_4;
extern char *main_image_5;
extern void (*grid_visit_func)();
extern char *haze_table_0;
extern char *haze_buffer_1;
extern char *quest_face_images;
extern char *face_images_1;
extern char *face_images_10;
extern char *face_images_11;
extern char *quest_face_images_special;
extern char *haze_table_1;
extern iptr shade_table_63;
extern char *haze_buffer_0;
extern char *hud_portrait_overlays;
extern char *compass_image;
extern unsigned spell_record_count;
extern char *compass_box_image;
extern int D_00195DA0;
extern char *qbn_opcode_arg_counts;
extern signed char mouse_control_mode;
extern unsigned char mouse_sensitivity_x;
extern unsigned char mouse_sensitivity_y;
extern char joystick_setting;
extern char joy_calibration_saved[];
extern char key_map[];
#ifdef DAGGER_PORT
/* the native build's control presets, once the controls are loaded (port/host/main.c) */
extern void port_controls_loaded(void);
#endif
extern int maps_bsa;
extern char *translucent_buffer;
extern char *water_tint_buffer;
extern int arch3d_bsa;
extern int dagger_snd;
extern int archive_open(char *, iptr, int);
extern void faction_load_file(void);
extern void region_load_tables(void);
extern int automap_button_view_mode(void);
extern void sky_init(void);
extern void weather_roll(void);
extern int text_rsc_open(void);
extern void msgbox_load_borders(void);
extern void init_world_objects(void);
extern void fatal_error(char *);
extern void flats_cfg_load(void);
extern void spell_cast_anims_load(void);
extern void books_find_path(void);
extern void init_link_caster(void);
extern void books_scan(void);
extern void magic_def_load(void);
extern int sound_init_music(void);
extern char *disk_read_file(char *, char *);
extern int disk_open_data(char *);
extern int disk_file_exists(char *);
extern void music_select_tables(void);
extern void automap_delete_files(void);
extern int rand_range(int, int);
extern void func_0007E066(void);
extern int func_0007EEAF(void);
extern void intrface_init(void);
extern void intrface_set_regions(void);
extern void model_heap_init(int);
extern void object_foreach_open(struct record *, void (*)());
extern void color_init_remap_tables(void);
extern void xn_rand_noise_init(void);
extern void xn_sky_snow_init(void);
extern char *xn_shade_table_63(void);
extern int xn_shade_init_reserved(void);
extern int xn_shade_build_translucent_table(char *);
extern char *xn_mem_align_up(char *, int);
extern int xn_mouse_set_sensitivity(short, short);
extern int xn_mouse_get_sensitivity(unsigned char *, unsigned char *);
extern void *xn_font_load(int, int);
extern int xn_font_select(int);
extern void xn_water_init(void);
extern void xn_tex_cache_init(unsigned);
extern void xn_joy_init(void);
extern void xn_joy_reset_range(void);
extern void xn_joy_calibrate(void);
extern int xn_helmet_open(int, int);
#pragma aux mc_set_location parm routine [];

void init_game_data(void)
{
    int fd;
    short unused;
    short h;
    short w;
    short i;
    int flag;

    flag = 0;
    srand(*(int *)DOS_LOW(0x46C));
    xn_font_load(0, 1);
    xn_font_load(1, 2);
    xn_font_load(2, 3);
    xn_font_load(3, 4);
    xn_font_select(4);
    msgbox_load_borders();
    text_rsc_open();
    grid_visit_func = object_foreach_open;
    buttons_rci = disk_read_file(str_buttons_rci, 0);
    icon_image = disk_read_file(str_icon00i0_img, 0);
    spell_records = (struct spell *)disk_read_file(str_spells_std_2, 0);
    spell_record_count = disk_last_file_size / 89;
    hud_bar_image = disk_read_file(str_main00i0_img, 0);
    hud_vital_bar_images = disk_read_file(str_main03i0_img, 0);
    main_image_4 = disk_read_file(str_main04i0_img, 0);
    main_image_5 = disk_read_file(str_main05i0_img, 0);
    hud_portrait_overlays = disk_read_file(str_mcol00i0_cif, 0);
    mc_set_location(85, str_init_c);
    mc_sprintf(((char *)text_buffer), str_rumor_dat_3, arena2_path);
    unlink(((char *)text_buffer));
    spell_cast_anims_load();
    hud_compass_image = disk_read_file(str_cmpa00i0_bss, 0);
    compass_frames_1 = disk_read_file(str_cmpa01i0_bss, 0);
    compass_frames_2 = disk_read_file(str_cmpa02i0_bss, 0);
    compass_frame_image = disk_read_file(str_cmpa03i0_img, 0);
    hud_mode_icons = disk_read_file(str_main01i0_img, 0);
    hud_portrait = mc_malloc(2048, str_init_c, 95);
    paperdoll_buffer = mc_malloc(24625, str_init_c, 96);
    paperdoll_mask = mc_malloc(24625, str_init_c, 97);
    ui_dirty_flags |= 8;
    quest_face_images = disk_read_file(str_face00i0_cif, 0);
    face_images_1 = disk_read_file(str_face01i0_cif, 0);
    face_images_10 = disk_read_file(str_face10i0_cif, 0);
    face_images_11 = disk_read_file(str_face11i0_cif, 0);
    quest_face_images_special = disk_read_file(str_kids00i0_cif, 0);
    horse_overlay_image = disk_read_file(str_mred00i0_cfa, 0);
    cart_overlay_image = disk_read_file(str_mred01i0_cfa, 0);
    compass_box_image = disk_read_file(str_compbox_img, 0);
    compass_image = disk_read_file(str_compass_img, 0);
    qbn_opcode_arg_counts = disk_read_file(str_count_dat, 0);
    flats_cfg_load();
    magic_def_load();
    books_find_path();
    books_scan();
    intrface_init();
    xn_tex_cache_init(cfg_texture_memory << 10);
    arch3d_bsa = archive_open(str_arch3d_bsa, 0, 0);
    maps_bsa = archive_open(str_maps_bsa, 0, 0);
    dagger_snd = archive_open(str_dagger_snd, 0, 0);
    monster_bsa_handle = archive_open(str_monster_bsa, 0, 0);
    model_heap_init(819200);
    intrface_set_regions();
    color_init_remap_tables();
    func_0007EEAF();
    xn_joy_init();
    xn_joy_reset_range();
    joystick_status = 1;
    xn_joy_calibrate();
    joystick_status = 2;
    mc_memcpy(key_map, default_key_map, 38, str_init_c, 145, 38);
    fd = disk_open_data(controls_file);
    if (fd > 0) {
        read(fd, ((char *)&mouse_control_mode), 54);
        read(fd, key_map, 38);
        w = mouse_sensitivity_x * 6;
        h = mouse_sensitivity_y * 6;
        if (h < 6)
            h = 6;
        if (w < 6)
            w = 6;
        xn_mouse_set_sensitivity(w, h);
        close(fd);
        mc_memcpy(xn_joy_calibration, joy_calibration_saved, 46, str_init_c, 158, 4);
        joystick_status = joystick_setting;
    } else {
        xn_mouse_get_sensitivity(&mouse_sensitivity_x, &mouse_sensitivity_y);
        mouse_sensitivity_x /= 6;
        mouse_sensitivity_y /= 6;
    }
#ifdef DAGGER_PORT
    port_controls_loaded();
#endif
    init_world_objects();
    region_load_tables();
    sky_init();
    xn_sky_snow_init();
    weather_roll();
    last_skill_check_minutes = game_minutes = 523530;
    for (i = 0; i < 20; i++)
        weather_roll();
    unlink(str_test_dat);
    faction_load_file();
    for (i = 0; i < 62; i++)
        regions[i].price_adjustment = rand_range(0, 500) + 750;
    if (sound_init_music() == 0)
        flag = 1;
    xn_water_init();
    water_tint_buffer = mc_malloc(512, str_init_c, 186);
    disk_read_file(str_water_tbl, xn_water_tint_table = xn_mem_align_up(water_tint_buffer, 256));
    translucent_buffer = mc_malloc(4352, str_init_c, 190);
    xn_shade_build_translucent_table(xn_shade_translucent_table = xn_mem_align_up(translucent_buffer, 256));
    shade_table_63 = (iptr)xn_shade_table_63();
    haze_buffer_0 = mc_malloc(16640, str_init_c, 196);
    disk_read_file(str_haze_000, haze_table_0 = xn_mem_align_up(haze_buffer_0, 256));
    haze_buffer_1 = mc_malloc(16640, str_init_c, 200);
    disk_read_file(str_haze_001, haze_table_1 = xn_mem_align_up(haze_buffer_1, 256));
    xn_shade_init_reserved();
    realtime_clock_tick = *(int *)DOS_LOW(0x46C);
    func_0007E066();
    if (cfg_helmet != 0) {
        if (cfg_helmet == 1)
            xn_helmet_open(1, 4);
        else
            xn_helmet_open(1, 0);
        xn_helmet_smoothing = 4;
    }
    if (disk_file_exists(str_dagger_grd_2) != 0) {
        *game_settings |= 4;
        D_00195DA0 = 434;
    } else {
        D_00195DA0 = 380;
    }
    music_select_tables();
    automap_delete_files();
    automap_button_view_mode();
    init_link_caster();
    xn_rand_noise_init();
    if (flag != 0)
        fatal_error(str_unable_to_initialize_the_sound_drivers);
}
