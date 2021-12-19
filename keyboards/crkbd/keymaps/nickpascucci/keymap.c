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

enum custom_keycodes {
    KC_REPT = SAFE_RANGE // Repeat last key
};

// Layer order macros for ease of reference.
#define L_COLE  0
#define L_NAV   1
#define L_NUM   2
#define L_FUN   3

// Define some helper macros for mod-tap keys. These just give shorter names to these keycodes so
// that the keymap stays nicely aligned below. They are defined in order from most-interior to
// most-exterior.
#define MH(kc) HYPR_T(KC_ ## kc)
#define MS(kc) LSFT_T(KC_ ## kc)
#define MG(kc) LGUI_T(KC_ ## kc)
#define MC(kc) LCTL_T(KC_ ## kc)
#define MA(kc) LALT_T(KC_ ## kc)

#define OSM_ALT OSM(MOD_LALT)
#define OSM_CTL OSM(MOD_LCTL)
#define OSM_SFT OSM(MOD_LSFT)
#define OSM_GUI OSM(MOD_LGUI)

#define THM_1 MH(ESC)
#define THM_2 LT(L_NAV,   KC_BSPC)
#define THM_3 MG(TAB)

#define THM_4 MG(ENT)
#define THM_5 LT(L_NUM,   KC_SPC)
#define THM_6 MH(MINS)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_COLE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      OSM_ALT,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                         KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, KC_REPT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      OSM_SFT,   MA(A),   MG(R),   MC(S),   MS(T),   MH(G),                        MH(M),   MS(N),   MC(E),   MG(I),   MA(O), KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      OSM_CTL,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                         KC_K,    KC_H, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            THM_1,   THM_2,   THM_3,      THM_4,   THM_5,   THM_6 
                                      //|--------+--------+--------|  |--------+--------+--------|

  ),


  [L_NAV] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      OSM_ALT, KC_WH_D, KC_BTN1, KC_MS_U, KC_BTN2, KC_ACL2,                      XXXXXXX, KC_PSTE, KC_COPY,  KC_CUT, KC_UNDO, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      OSM_SFT, KC_WH_U, KC_MS_L, KC_MS_D, KC_MS_R, KC_ACL1,                      KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_AGIN, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      OSM_CTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_ACL0,                       KC_END, KC_PGDN, KC_PGUP, KC_HOME, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    KC_VOLD, KC_MPLY, KC_VOLU 
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_NUM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      OSM_ALT, KC_EXLM,  KC_DLR, KC_AMPR, KC_ASTR, KC_BSLS,                       KC_GRV,    KC_7,    KC_8,    KC_9, KC_PLUS, OSL(L_FUN),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      OSM_SFT,  KC_EQL, KC_LBRC, KC_LCBR, KC_LPRN, KC_LABK,                      KC_HASH,    KC_4,    KC_5,    KC_6,    KC_0, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      OSM_CTL, KC_PERC, KC_RBRC, KC_RCBR, KC_RPRN, KC_RABK,                        KC_AT,    KC_1,    KC_2,    KC_3, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_MINS, KC_BSPC,  KC_TAB,    XXXXXXX, XXXXXXX, XXXXXXX 
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),


  [L_FUN] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_F12,   KC_F7,   KC_F8,   KC_F9, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_F11,   KC_F4,   KC_F5,   KC_F6, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_F10,   KC_F1,   KC_F2,   KC_F3, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX 
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
#endif // OLED_ENABLE

// Repeat last key along with modifiers.
// https://gist.github.com/NotGate/3e3d8ab81300a86522b2c2549f99b131

// Used to extract the basic tapping keycode from a dual-role key.
// Example: GET_TAP_KC(MT(MOD_RSFT, KC_E)) == KC_E
#define GET_TAP_KC(dual_role_key) dual_role_key & 0xFF
uint16_t last_keycode = KC_NO;
uint8_t last_modifier = 0;

// Initialize variables holding the bitfield
// representation of active modifiers.
uint8_t mod_state;
uint8_t oneshot_mod_state;

void process_repeat_key(uint16_t keycode, const keyrecord_t *record) {
    if (keycode != KC_REPT) {
        // Early return when holding down a pure layer key
        // to retain modifiers
        switch (keycode) {
            case QK_DEF_LAYER ... QK_DEF_LAYER_MAX:
            case QK_MOMENTARY ... QK_MOMENTARY_MAX:
            case QK_LAYER_MOD ... QK_LAYER_MOD_MAX:
            case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
            case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
            case QK_TO ... QK_TO_MAX:
            case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
                return;
        }
        last_modifier = oneshot_mod_state > mod_state ? oneshot_mod_state : mod_state;
        switch (keycode) {
            case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            case QK_MOD_TAP ... QK_MOD_TAP_MAX:
                if (record->event.pressed) {
                    last_keycode = GET_TAP_KC(keycode);
                }
                break;
            default:
                if (record->event.pressed) {
                    last_keycode = keycode;
                }
                break;
        }
    } else { // keycode == KC_REPT
        if (record->event.pressed) {
            register_mods(last_modifier);
            register_code16(last_keycode);
        } else {
            unregister_code16(last_keycode);
            unregister_mods(last_modifier);
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    process_repeat_key(keycode, record);
    // It's important to update the mod variables *after* calling process_repeat_key, or else
    // only a single modifier from the previous key is repeated (e.g. Ctrl+Shift+T then Repeat produces Shift+T)
    mod_state = get_mods();
    oneshot_mod_state = get_oneshot_mods();
    return true;
}
