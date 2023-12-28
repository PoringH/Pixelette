// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "print.h"
#include <qp.h>
#include QMK_KEYBOARD_H
#include "font/unlearned42.qff.h"
#include "font/minecraft20.qff.h"
#include "font/origami40.qff.h"



static painter_device_t display;
static painter_font_handle_t font1 = NULL;
static painter_font_handle_t font2 = NULL;
static painter_font_handle_t font3 = NULL;
static const char *text;
int16_t width;
uint8_t height;
uint8_t heightF2;
int16_t widthL2;
int vertpos = 0;
int hortpos = 0;
// int current_wpm = 0;
int x;
int y;
// int fonttype;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_75_ansi(
        KC_MUTE, KC_MPLY, KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13,           KC_DEL,
                          KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
                          KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_END,
                          KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_F14,  KC_F15,  KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_PGDN,
        QK_BOOT, KC_F16,  KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, KC_RGUI, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    )
};

#if defined(ENCODER_MAP_ENABLE)

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT)}
};

#endif


uint32_t deffered_init(uint32_t trigger_time, void *cb_arg) {
    display = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 16, 3);
    qp_init(display, QP_ROTATION_0);
    qp_power(display, true);
    qp_set_viewport_offsets(display, 35, 0);
	
    font1 = qp_load_font_mem(font_unlearned42);
    font2 = qp_load_font_mem(font_minecraft20);
    font3 = qp_load_font_mem(font_origami40);
    heightF2 = font2 -> line_height;
    widthL2 = qp_textwidth(font1, "LAYER 2");
    qp_rect(display, 0, 0, 170, 320, 0, 0, 0, true);
    qp_flush(display);
//    for debugging purposes
//	debug_enable=true;
//  	debug_matrix=true;
//  	text = "W";
//  	width = qp_textwidth(font, text);
//	height = font -> line_height;
//	vertpos = 160 - height/2;
//	hortpos = 85 - width/2;
  	
    return 0;
}

void keyboard_post_init_kb(void) {
    // debug_enable = true;
    defer_exec(200, deffered_init, NULL);
}  

void render_windows_logo(int x, int y)
{
    //square top left
    qp_rect(display, x, y, x+20, y+20, 0, 0, 255, true);
    //top right square
    qp_rect(display, x+22, y, x+42, y+20, 0, 0, 255, true);
    //bottom left square
    qp_rect(display, x, y+22, x+20, y+42, 0, 0, 255, true);
    //bottom right square
    qp_rect(display, x+22, y+22, x+42, y+42, 0, 0, 255, true);
}

void render_led_status(void)
{
    int usb_statex = 6;
    int cap_y = 65;
    int num_y = 134;
    int scroll_y = 202;
    int status_width = 42;
    int status_height = 54;
	led_usb_state = host_keyboard_led_state();
	if (led_usb_state.caps_lock == true){
		qp_rect(display, usb_statex, cap_y, usb_statex + status_width, cap_y + status_height, 130, 255, 255, true);
	}
	else{
		qp_rect(display, usb_statex, cap_y, usb_statex + status_width, cap_y + status_height, 0, 255, 255, true);
	}
	if (led_usb_state.num_lock == true){
		qp_rect(display, usb_statex, num_y, usb_statex + status_width, num_y + status_height, 130, 255, 255, true);
	}
	else{
		qp_rect(display, usb_statex, num_y, usb_statex + status_width, num_y + status_height, 0, 255, 255, true);
	}
	if (led_usb_state.scroll_lock == true){
		qp_rect(display, usb_statex, scroll_y, usb_statex + status_width, scroll_y + status_height, 130, 255, 255, true);
	}
	else{
		qp_rect(display, usb_statex, scroll_y, usb_statex + status_width , scroll_y + status_height , 0, 255, 255, true);
	}
}

led_t    led_usb_state;
void housekeeping_task_user(void) {

    qp_rect(display, 0, 53, 170, 55, 0, 0, 255, true);
    qp_rect(display, 54, 55, 56, 266, 0, 0, 255, true);
    qp_rect(display, 0, 266, 170, 268, 0, 0, 255, true);
    render_led_status();
    //layer indicator text
//    vertpos = 26 - heightF2/2;
//    hortpos = 85 - widthL2/2;
    //dynamic letters
	vertpos = 160 - height/2;
	hortpos = 113 - width/2;
	qp_drawtext(display, hortpos, vertpos, font3, text);
    render_windows_logo(6, 273);
    qp_drawtext(display, 54, 273, font2, "WINKEY");
    qp_drawtext(display, 54, 300, font2, "DISABLED");
//	for debugging purposes
//	if(qp_drawtext(display, hortpos, vertpos, font, text) < 0) {
//		uprintf("%s:wide\n", text);
//		uprintf("%s:%d width\n", text, width);
//		uprintf("%s:%d hortpos\n", text, hortpos);
//	}
//	else {
//		uprintf("%s:ok\n", text);
//		uprintf("%s:%d width\n", text, width);
//		uprintf("%s:%d hortpos\n", text, hortpos);
//		print("ok");
//	}
	
	
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch(keycode) {
		case KC_MUTE:
            if (record->event.pressed) {
                text = "MTE";
            }
            break;
        case KC_MPLY:
            if (record->event.pressed) {
                text = "PLY";
            }
            break;
        case KC_ESC:
            if (record->event.pressed) {
                text = "ESC";
            }
            break;
        case KC_F1:
            if (record->event.pressed) {
                text = "F1";
            }
            break;
        case KC_F2:
            if (record->event.pressed) {
                text = "F2";
            }
            break;
        case KC_F3:
            if (record->event.pressed) {
                text = "F3";
            }
            break;
        case KC_F4:
            if (record->event.pressed) {
                text = "F4";
            }
            break;
        case KC_F5:
            if (record->event.pressed) {
                text = "F5";
            }
            break;
        case KC_F6:
            if (record->event.pressed) {
                text = "F6";
            }
            break;
        case KC_F7:
            if (record->event.pressed) {
                text = "F7";
            }
            break;
        case KC_F8:
            if (record->event.pressed) {
                text = "F8";
            }
            break;
        case KC_F9:
            if (record->event.pressed) {
                text = "F9";
            }
            break;
        case KC_F10:
            if (record->event.pressed) {
                text = "F10";
            }
            break;
        case KC_F11:
            if (record->event.pressed) {
                text = "F11";
            }
            break;
        case KC_F12:
            if (record->event.pressed) {
                text = "F12";
            }
            break;
        case KC_F13:
            if (record->event.pressed) {
                text = "F13";
            }
            break;
        case KC_F14:
            if (record->event.pressed) {
                text = "F14";
            }
            break;
        case KC_F15:
            if (record->event.pressed) {
                text = "F15";
            }
            break;
        case KC_F16:
            if (record->event.pressed) {
                text = "F16";
            }
            break;
        case KC_DEL:
            if (record->event.pressed) {
                text = "DEL";
            }
            break;
        case KC_GRV:
            if (record->event.pressed) {
                text = "~";
            }
            break;
        case KC_1:
            if (record->event.pressed) {
                text = "1";
            }
            break;
        case KC_2:
            if (record->event.pressed) {
                text = "2";
            }
            break;
        case KC_3:
            if (record->event.pressed) {
                text = "3";
            }
            break;
        case KC_4:
            if (record->event.pressed) {
                text = "4";
            }
            break;
        case KC_5:
            if (record->event.pressed) {
                text = "5";
            }
            break;
        case KC_6:
            if (record->event.pressed) {
                text = "6";
            }
            break;
        case KC_7:
            if (record->event.pressed) {
                text = "7";
            }
            break;
        case KC_8:
            if (record->event.pressed) {
                text = "8";
            }
            break;
        case KC_9:
            if (record->event.pressed) {
                text = "9";
            }
            break;
        case KC_0:
            if (record->event.pressed) {
                text = "0";
            }
            break;
        case KC_MINS:
            if (record->event.pressed) {
                text = "-";
            }
            break;
        case KC_EQL:
            if (record->event.pressed) {
                text = "=";
            }
            break;
        case KC_BSPC:
            if (record->event.pressed) {
                text = "BPC";
            }
            break;
        case KC_HOME:
            if (record->event.pressed) {
                text = "HOM";
            }
            break;
        case KC_END:
            if (record->event.pressed) {
                text = "END";
            }
            break;
//        case KC_BSLS:
//            if (record->event.pressed) {
//                text = "\";
//            }
//            break;
        case KC_RBRC:
            if (record->event.pressed) {
                text = "]";
            }
            break;
        case KC_LBRC:
            if (record->event.pressed) {
                text = "[";
            }
            break;
        case KC_TAB:
            if (record->event.pressed) {
                text = "TAB";
            }
            break;
        case KC_CAPS:
            if (record->event.pressed) {
                text = "CAP";
            }
            break;
        case KC_SCLN:
            if (record->event.pressed) {
                text = ";";
            }
            break;
        case KC_QUOT:
            if (record->event.pressed) {
                text = "'";
            }
            break;
        case KC_ENT:
            if (record->event.pressed) {
                text = "ENT";
            }
            break;
        case KC_PGUP:
            if (record->event.pressed) {
                text = "PUP";
            }
            break;
        case KC_PGDN:
            if (record->event.pressed) {
                text = "PDN";
            }
            break;
        case KC_UP:
            if (record->event.pressed) {
                text = "UP";
            }
            break;
        case KC_RSFT:
            if (record->event.pressed) {
                text = "SFT";
            }
            break;
//        case KC_SLSH:
//            if (record->event.pressed) {
//                text = "/";
//            }
//            break;
        case KC_DOT:
            if (record->event.pressed) {
                text = ".";
            }
            break;
        case KC_COMM:
            if (record->event.pressed) {
                text = ",";
            }
            break;
        case KC_LSFT:
        	if (record->event.pressed) {
                text = "SFT";
            }
            break;
        case KC_LCTL:
            if (record->event.pressed) {
                text = "CTL";
            }
            break;
        case KC_LGUI:
            if (record->event.pressed) {
                text = "GUI";
            }
            break;
        case KC_LALT:
            if (record->event.pressed) {
                text = "ALT";
            }
            break;
        case KC_SPC:
            if (record->event.pressed) {
                text = "SPC";
            }
            break;
        case KC_RALT:
            if (record->event.pressed) {
                text = "]";
            }
            break;
    	case KC_RCTL:
            if (record->event.pressed) {
                text = "CLT";
            }
            break;
        case KC_LEFT:
            if (record->event.pressed) {
                text = "LFT";
            }
            break;
        case KC_DOWN:
            if (record->event.pressed) {
                text = "DWN";
            }
            break;
        case KC_RGHT:
            if (record->event.pressed) {
                text = "RGT";
            }
            break;
        case KC_A:
            if (record->event.pressed) {
                text = "A";
            }
            break;
        case KC_B:
            if (record->event.pressed) {
                text = "B";
            }
            break;
        case KC_C:
            if (record->event.pressed) {
                text = "C";
            }
            break;
        case KC_D:
            if (record->event.pressed) {
                text = "D";
            }
            break;
        case KC_E:
            if (record->event.pressed) {
                text = "E";
            }
            break;
        case KC_F:
            if (record->event.pressed) {
                text = "F";
            }
            break;
        case KC_G:
            if (record->event.pressed) {
                text = "G";
            }
            break;
        case KC_H:
            if (record->event.pressed) {
                text = "H";
            }
            break;
        case KC_I:
            if (record->event.pressed) {
//				qp_clear(display);
                text = "I";
            }
            break;
        case KC_J:
            if (record->event.pressed) {
                text = "J";
            }
            break;
        case KC_K:
            if (record->event.pressed) {
                text = "K";
            }
            break;
        case KC_L:
            if (record->event.pressed) {
                text = "L";
            }
            break;
        case KC_M:
            if (record->event.pressed) {
                text = "M";
            }
            break;
        case KC_N:
            if (record->event.pressed) {
                text = "N";
            }
            break;
        case KC_O:
            if (record->event.pressed) {
                text = "O";
            }
            break;
        case KC_P:
            if (record->event.pressed) {
                text = "P";
            }
            break;
        case KC_Q:
            if (record->event.pressed) {
                text = "Q";
            }
            break;
        case KC_R:
            if (record->event.pressed) {
                text = "R";
            }
            break;
        case KC_S:
            if (record->event.pressed) {
                text = "S";
            }
            break;
        case KC_T:
            if (record->event.pressed) {
                text = "T";
            }
            break;
        case KC_U:
            if (record->event.pressed) {
                text = "U";
            }
            break;
        case KC_V:
            if (record->event.pressed) {
                text = "V";
            }
            break;
        case KC_W:
            if (record->event.pressed) {
                text = "W";
            }
            break;
        case KC_X:
            if (record->event.pressed) {
                text = "X";
            }
            break;
        case KC_Y:
            if (record->event.pressed) {
                text = "Y";
            }
            break;
        case KC_Z:
            if (record->event.pressed) {
                text = "Z";
            }
            break;
        default:
        	break;
    }
    if(qp_textwidth(font3, text) < width)
    {
    	qp_rect(display, hortpos, vertpos, hortpos+width, vertpos+height, 0, 0, 0, true);
	}
    
    width = qp_textwidth(font3, text);
	height = font3 -> line_height;
    return true;
};
