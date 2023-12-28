// Copyright 2023 PoringH (@PoringH)
// SPDX-License-Identifier: GPL-2.0-or-later

//#include <qp.h>
//
//static painter_device_t lcd;
////static painter_image_handle_t background;
////static painter_image_handle_t animated;
//
//
//void keyboard_post_init_kb(void) {
////    background = qp_load_image_mem(gfx_background);
////    animated = qp_load_image_mem(gfx_animated);
//    
//    // Give the LCD some time to get some power
//    wait_ms(150);
//    
//    // Initialise the LCD
//    lcd = qp_st7789_make_spi_device(170, 320, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 4, 0);
//    qp_init(lcd, QP_ROTATION_180);
//    // Turn on the LCD
//    qp_power(lcd, false);
//    qp_rect(lcd, 0, 0, 320, 170, 0, 0, 0, true);
//    qp_flush(lcd);
//    static uint32_t last_draw = 0;
//    if (timer_elapsed32(last_draw) > 33) { // Throttle to 30fps
//        last_draw = timer_read32();
//        // Draw 8px-wide rainbow filled rectangles down the left side of the display
//        for (int i = 0; i < 239; i+=8) {
//            qp_rect(lcd, 0, i, 7, i+7, i, 255, 255, true);
//        }
//        qp_flush(lcd);
//    }
//    // Draw image
////    qp_drawimage(lcd, 0, 0, background);
////    draw_ui_init_user();
//    
//    keyboard_post_init_user();
//}

