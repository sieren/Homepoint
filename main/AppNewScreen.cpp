#include "AppNewScreen.h"
#include <config/PlatformInject.hpp>
extern "C"
{
  #include "lvgl.h"
  #include "lvgl_helpers.h"
  #include "axp192.h"
}

#define LV_TICK_PERIOD_MS 1
#include <memory>

static void gui_timer_tick(void *arg)
{
	// Unused
	(void) arg;

	lv_tick_inc(LV_TICK_PERIOD_MS);
}

namespace gfx 
{
  AppNewScreen::AppNewScreen(std::shared_ptr<ctx::AppContext> ctx) :
    mpAppContext(ctx)
  {
    
  }

  void AppNewScreen::init()
  {
    static lv_color_t bufs[2][DISP_BUF_SIZE];
    static lv_disp_buf_t disp_buf;
    uint32_t size_in_px = DISP_BUF_SIZE;

    // Set up the frame buffers
    lv_disp_buf_init(&disp_buf, &bufs[0], &bufs[1], size_in_px);

    // Set up the display driver
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_t indev_drv;
	  lv_indev_drv_init(&indev_drv);
	  indev_drv.read_cb = touch_driver_read;
	  indev_drv.type = LV_INDEV_TYPE_POINTER;
	  lv_indev_drv_register(&indev_drv);
  
    const esp_timer_create_args_t periodic_timer_args = {
      .callback = &gui_timer_tick,
      .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

//     lv_obj_t * bar = lv_bar_create(lv_layer_sys(), NULL);
//     lv_obj_set_hidden(bar, true);
//     lv_bar_set_range(bar, 0, 100);
//     lv_bar_set_value(bar, 10, LV_ANIM_OFF);
//     lv_obj_set_size(bar, 200, 15);
//     lv_obj_align(bar, lv_layer_sys(), LV_ALIGN_CENTER, 0, -10);
//   //  lv_obj_user_data_t udata = (lv_obj_user_data_t){10, 0, 10};
//  //   lv_obj_set_user_data(bar, NULL);
//     lv_obj_set_style_local_value_color(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_COLOR_WHITE);
//     lv_obj_set_style_local_value_align(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_CENTER);
//     lv_obj_set_style_local_value_ofs_y(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, 20);
//     lv_obj_set_style_local_value_font(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_THEME_DEFAULT_FONT_NORMAL);
//     lv_obj_set_style_local_bg_color(lv_layer_sys(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
//     lv_obj_set_style_local_bg_opa(lv_layer_sys(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
//     lv_obj_t * scr = lv_disp_get_scr_act(NULL);
    // lv_obj_t * label1 =  lv_label_create(scr, NULL);

    // /*Modify the Label's text*/
    // lv_label_set_text(label1, "Hello\nworld");

    // /* Align the Label to the center
    //  * NULL means align on parent (which is the screen now)
    //  * 0, 0 at the end means an x, y offset after alignment*/
    // lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);    

//     lv_obj_t *btn1 = lv_btn_create(scr, NULL);           /*Add to the active screen*/
//     lv_obj_set_pos(btn1, 2, 2);                                    /*Adjust the position*/
//     lv_obj_set_size(btn1, 96, 96);                                 /* set size of button */
//     lv_obj_set_event_cb(btn1, [](lv_obj_t * obj, lv_event_t event)
//     {
//       Serial.println("Test Button");
//     });

    static lv_style_t style_bullet;
    lv_style_init(&style_bullet);
    lv_style_set_border_width(&style_bullet, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_inner(&style_bullet, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&style_bullet, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_bullet, LV_STATE_DEFAULT, 0);
  
    mpMainMenu = lv_cont_create(lv_scr_act(), NULL);
    lv_cont_set_layout(mpMainMenu, LV_LAYOUT_COLUMN_MID);
    lv_obj_add_style(mpMainMenu, LV_OBJ_PART_MAIN, &style_bullet);
    lv_obj_set_auto_realign(mpMainMenu, true);                    /*Auto realign when the size changes*/
    lv_obj_align_origo(mpMainMenu, NULL, LV_ALIGN_CENTER, 0, 0);  /*This parametrs will be sued when realigned*/
    lv_cont_set_fit(mpMainMenu, LV_FIT_MAX);
    lv_cont_set_layout(mpMainMenu, LV_LAYOUT_COLUMN_MID);
    mpStatusBar = std::make_shared<UIStatusBar>(UIStatusBar(mpMainMenu));
    mpStatusBar->registerCallback(&mpAppContext->getWifiContext());

    static lv_point_t valid_pos[] = {{0,0}, {0, 1}, {1,1}};
    lv_obj_t *tileview;
    tileview = lv_tileview_create(mpMainMenu, NULL);
    lv_tileview_set_valid_positions(tileview, valid_pos, 3);
    lv_tileview_set_edge_flash(tileview, true);
    static lv_style_t tileStyle;
    lv_style_copy(&tileStyle, &style_bullet);
    lv_style_set_pad_inner(&tileStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&tileStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&tileStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&tileStyle, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(tileview, LV_OBJ_PART_MAIN, &tileStyle);

    lv_obj_t * tile1 = lv_obj_create(tileview, NULL);
    lv_obj_set_size(tile1, LV_HOR_RES, LV_VER_RES);
    lv_tileview_add_element(tileview, tile1);

    /*Tile1: just a label*/
    lv_obj_t * label = lv_label_create(tile1, NULL);
    lv_label_set_text(label, "Scroll down");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

    /*Tile2: a list*/
    lv_obj_t * list = lv_list_create(tileview, NULL);
    lv_obj_set_size(list, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(list, 0, LV_VER_RES);
    lv_list_set_scroll_propagation(list, true);
    lv_list_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);

    lv_list_add_btn(list, NULL, "One");
    lv_list_add_btn(list, NULL, "Two");
    lv_list_add_btn(list, NULL, "Three");
    lv_list_add_btn(list, NULL, "Four");
    lv_list_add_btn(list, NULL, "Five");
    lv_list_add_btn(list, NULL, "Six");
    lv_list_add_btn(list, NULL, "Seven");
    lv_list_add_btn(list, NULL, "Eight");

    /*Tile3: a button*/
    lv_obj_t * tile3 = lv_obj_create(tileview, tile1);
    lv_obj_set_pos(tile3, LV_HOR_RES, LV_VER_RES);
    lv_tileview_add_element(tileview, tile3);

    lv_obj_t * btn = lv_btn_create(tile3, NULL);
    lv_obj_align(btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_tileview_add_element(tileview, btn);
    label = lv_label_create(btn, NULL);
    lv_label_set_text(label, "No scroll up");
    Serial.println("Done setting up the GUI");
  }

  void AppNewScreen::loop()
  {
    lv_task_handler(); // process animations
  }
} // namespace gfx