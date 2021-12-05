/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>

/*
 * This layout is loosely inspired by Miryoku, but with a few modifications.
 */

// Layer order macros for ease of reference.
#define L_COLE  0
#define L_MEDIA 1
#define L_NAV   2
#define L_MOUSE 3
#define L_SYM   4
#define L_NUM   5
#define L_FUN   6

// Define some helper macros for mod-tap keys. These just give shorter names to these keycodes so
// that the keymap stays nicely aligned below. They are defined in order from most-interior to
// most-exterior.
#define MH(kc) HYPR_T(kc)
#define MS(kc) LSFT_T(kc)
#define MG(kc) LGUI_T(kc)
#define MC(kc) LCTL_T(kc)
#define MA(kc) LALT_T(kc)

#define THM_1 LT(L_MEDIA, KC_ESC)
#define THM_2 LT(L_NAV,   KC_BSPC)
#define THM_3 LT(L_MOUSE, KC_TAB)
#define THM_4 LT(L_SYM,   KC_ENT)
#define THM_5 LT(L_NUM,   KC_SPC)
#define THM_6 LT(L_FUN,   KC_EQL)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_COLE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                         KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_CAPS,MA(KC_A),MG(KC_R),MC(KC_S),MS(KC_T),MH(KC_G),                     MH(KC_M),MS(KC_N),MC(KC_E),MG(KC_I),MA(KC_O), KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                         KC_K,    KC_H, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            THM_1,   THM_2,   THM_3,      THM_4,   THM_5,   THM_6 
                                      //|--------+--------+--------|  |--------+--------+--------|

  ),

  [L_MOUSE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_PSTE, KC_COPY,  KC_CUT, KC_UNDO, KC_ACL2,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LALT, KC_LGUI, KC_LCTL, KC_LSFT, KC_HYPR,                      KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_AGIN, KC_ACL1,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_WH_L, KC_WH_U, KC_WH_D, KC_WH_R, KC_CAPS, KC_ACL0,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    KC_BTN1, KC_BTN2, KC_BTN3
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_NAV] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_PSTE, KC_COPY,  KC_CUT, KC_UNDO, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LALT, KC_LGUI, KC_LCTL, KC_LSFT, KC_HYPR,                      KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_AGIN, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_HOME, KC_PGDN, KC_PGUP,  KC_END, KC_CAPS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX 
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_MEDIA] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MPLY, KC_MPRV, KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LALT, KC_LGUI, KC_LCTL, KC_LSFT, KC_HYPR,                      KC_MUTE, KC_VOLD, KC_VOLU, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX 
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_SYM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, KC_EXLM,  KC_DLR, KC_AMPR, KC_ASTR, KC_BSLS,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_EQL,  KC_LBRC, KC_LCBR, KC_LPRN, KC_LABK,                      KC_HYPR, KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_PERC, KC_RBRC, KC_RCBR, KC_RPRN, KC_RABK,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_ESC, KC_BSPC, KC_UNDS,    XXXXXXX, XXXXXXX, XXXXXXX 
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_NUM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,  KC_GRV,    KC_7,    KC_8,    KC_9, KC_PLUS,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_HASH,    KC_4,    KC_5,    KC_6,  KC_EQL,                      KC_HYPR, KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,   KC_AT,    KC_1,    KC_2,    KC_3, KC_MINS,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_ESC, KC_BSPC,    KC_0,    XXXXXXX, XXXXXXX, XXXXXXX 
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_FUN] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,  KC_F12,   KC_F7,   KC_F8,   KC_F9, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,  KC_F11,   KC_F4,   KC_F5,   KC_F6, XXXXXXX,                      KC_HYPR, KC_LSFT, KC_LCTL, KC_LGUI, KC_LALT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,  KC_F10,   KC_F1,   KC_F2,   KC_F3, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_ESC, KC_BSPC, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX 
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}


void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
        if (IS_LAYER_ON(L_NAV)) {
            oled_write_ln_P(PSTR("Navigation"), false);
            return;
        }
        if (IS_LAYER_ON(L_MOUSE)) {
            oled_write_ln_P(PSTR("Mouse"), false);
            return;
        }
        if (IS_LAYER_ON(L_MEDIA)) {
            oled_write_ln_P(PSTR("Media"), false);
            return;
        }
        if (IS_LAYER_ON(L_SYM)) {
            oled_write_ln_P(PSTR("Symbols"), false);
            return;
        }
        if (IS_LAYER_ON(L_NUM)) {
            oled_write_ln_P(PSTR("Numpad"), false);
            return;
        }
        if (IS_LAYER_ON(L_FUN)) {
            oled_write_ln_P(PSTR("Function"), false);
            return;
        }
        // Note: these should be last so that if other layers are active they get priority.
        if (IS_LAYER_ON(L_COLE)) {
            oled_write_ln_P(PSTR("Colemak"), false);
            return;
        }
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
    } else {
        oled_render_logo();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}
#endif // OLED_ENABLE
