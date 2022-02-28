#include QMK_KEYBOARD_H
#include <stdio.h>

/*
 * This layout is loosely inspired by Miryoku, but with a few modifications.
 *
 * In order to guide the design of the symbol layers and combos, it is useful to have a sense of
 * which symbols are used most often. My "symcount" tool can be used to extract this from a corpus
 * of text. Running it over my current Rust and Python projects, these are the top 15
 * non-alphanumeric symbols:
 *
 * _, 59770
 * (, 43835
 * ), 43833
 * =, 22896
 * /, 19186
 * {, 14384
 * }, 14383
 * [, 11780
 * ], 11771
 * >, 10782
 * #, 10483
 * -, 10136
 * !, 9257
 * *, 8252
 * <, 5755
 * 
 * As expected, the standard parentheses and underscore characters used for function calls and for
 * identifiers are the top symbols followed by curly and square braces. Various bigrams, such as the
 * Rust comment prefix (//) and the single-arrow (->), are also present.
 */

// Layer order macros for ease of reference.
#define L_COLE  0
#define L_NAV   1
#define L_MOUS  2
#define L_SYM   3
#define L_NUM   4
#define L_FUN   5
#define L_MED   6

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

#define THM_1 KC_ESC
#define THM_2 LT(L_NAV,   KC_BSPC)
#define THM_3 LT(L_NUM,   KC_TAB)

#define THM_4 LT(L_MOUS,  KC_ENT)
#define THM_5 LT(L_SYM,   KC_SPC)
#define THM_6 LT(L_MED,   KC_EQL)

// Forward declaration for custom key functions
bool process_caps_word(uint16_t keycode, keyrecord_t* record);

static bool caps_word_enabled = false;
static bool shifted = false;

// Initialize variables holding the bitfield
// representation of active modifiers.
uint8_t mod_state;
uint8_t oneshot_mod_state;

typedef struct {
  uint16_t keycode;
  uint16_t shifted_keycode;
} custom_shift_key_t;

bool process_custom_shift_keys(uint16_t keycode, keyrecord_t *record);

const custom_shift_key_t custom_shift_keys[] = {
  {KC_LPRN, KC_RPRN}, // Shift ( is )
  {KC_LBRC, KC_RBRC}, // Shift [ is ]
  {KC_LCBR, KC_RCBR}, // Shift { is }
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

// Combos allow access to common symbols without shifting layers.
//
// In selecting keys to use as combos, it's best to avoid using the pinkies where possible - because
// they are so much weaker than the other fingers it makes it hard to hit both keys at once.

const uint16_t PROGMEM combo_wf[] = {KC_W, KC_F, COMBO_END};
const uint16_t PROGMEM combo_fp[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM combo_pb[] = {KC_P, KC_B, COMBO_END};

const uint16_t PROGMEM combo_jl[] = {KC_J, KC_L, COMBO_END};
const uint16_t PROGMEM combo_lu[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM combo_uy[] = {KC_U, KC_Y, COMBO_END};

const uint16_t PROGMEM combo_tn[] = {MS(T), MS(N), COMBO_END};

// Custom keys can be used for macros. These must be handled in the process_record_user function
// below.
enum custom_keycodes {
    KC_CAPW = SAFE_RANGE, // "caps-word"
    KC_ARRW, // arrow (->)
    KC_DRRW, // double arrow (=>)
    KC_DCLN, // double colon (::)
    KC_DAMP, // double ampersand (&&)
    KC_DPIP, // double pipe (||)
};

combo_t key_combos[COMBO_COUNT] = {
    // Upper row: programming bigrams
    COMBO(combo_wf, KC_DPIP),
    COMBO(combo_fp, LCTL(KC_6)), // Vim: change to alternate buffer
    COMBO(combo_pb, KC_DAMP),

    COMBO(combo_jl, KC_DCLN),
    COMBO(combo_lu, KC_ARRW),
    COMBO(combo_uy, KC_DRRW),

    // Caps Word
    COMBO(combo_tn, KC_CAPW),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_COLE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_GRV,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                         KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, KC_MINS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LPRN,   MA(A),   MG(R),   MC(S),   MS(T),   MH(G),                        MH(M),   MS(N),   MC(E),   MG(I),   MA(O), KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCBR,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                         KC_K,    KC_H, KC_COMM,  KC_DOT, KC_SLSH, KC_LBRC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            THM_1,   THM_2,   THM_3,      THM_4,   THM_5,   THM_6 
                                      //|--------+--------+--------|  |--------+--------+--------|

  ),

  [L_SYM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_BSLS, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_EQL, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,                      KC_HYPR, KC_RSFT, KC_RCTL, KC_RGUI, KC_RALT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, KC_BSPC,  KC_TAB,    XXXXXXX, XXXXXXX, XXXXXXX
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_NUM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,    KC_7,    KC_8,    KC_9, KC_PLUS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_LGUI, KC_LCTL, KC_LSFT, KC_HYPR,                      XXXXXXX,    KC_4,    KC_5,    KC_6,    KC_0, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,    KC_1,    KC_2,    KC_3, OSL(L_FUN), XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_NAV] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_PSTE, KC_COPY,  KC_CUT, KC_UNDO, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_AGIN, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_END, KC_PGDN, KC_PGUP, KC_HOME, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX,TO(L_MOUS) 
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_MOUS] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, KC_ACL2, KC_WH_D, KC_MS_U, KC_WH_U, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_ACL1, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_ACL0, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                       TO(L_COLE), XXXXXXX, XXXXXXX,    KC_BTN1, KC_BTN2, KC_BTN3
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_MED] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, KC_MPLY, KC_MUTE,    XXXXXXX, XXXXXXX, XXXXXXX
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!process_caps_word(keycode, record)) { return false; }
  if (!process_custom_shift_keys(keycode, record)) { return false; }

  mod_state = get_mods();
  oneshot_mod_state = get_oneshot_mods();

  switch (keycode) {
    case KC_CAPW:
      if (record->event.pressed) {
        caps_word_enabled = true;
      }
      return false;

    case KC_ARRW:
      if (record->event.pressed) {
        SEND_STRING("->");
      }
      return false;

    case KC_DRRW:
      if (record->event.pressed) {
        SEND_STRING("=>");
      }
      return false;

    case KC_DCLN:
      if (record->event.pressed) {
        SEND_STRING("::");
      }
      return false;

    case KC_DAMP:
      if (record->event.pressed) {
        SEND_STRING("&&");
      }
      return false;

    case KC_DPIP:
      if (record->event.pressed) {
        SEND_STRING("||");
      }
      return false;
  }

  return true;
}

// Copied from https://getreuer.info/posts/keyboards/caps-word/index.html
bool process_caps_word(uint16_t keycode, keyrecord_t* record) {
  if (!caps_word_enabled) {
    return true;
  }

  if (!record->event.pressed) { return true; }

  if (!((get_mods() | get_oneshot_mods()) & ~MOD_MASK_SHIFT)) {
    switch (keycode) {
      case QK_MOD_TAP ... QK_MOD_TAP_MAX:
      case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        // Earlier return if this has not been considered tapped yet.
        if (record->tap.count == 0) { return true; }
        // Get the base tapping keycode of a mod- or layer-tap key.
        keycode &= 0xff;
    }

    switch (keycode) {
      // Letter keys should be shifted.
      case KC_A ... KC_Z:
        if (!shifted) { register_code(KC_LSFT); }
        shifted = true;
        return true;

      // Keycodes that continue caps word but shouldn't get shifted.
      case KC_1 ... KC_0:
      case KC_BSPC:
      case KC_MINS:
      case KC_UNDS:
        if (shifted) { unregister_code(KC_LSFT); }
        shifted = false;
        return true;

      // Any other keycode disables caps word.
    }
  }

  // Disable caps word.
  caps_word_enabled = false;
  if (shifted) { unregister_code(KC_LSFT); }
  shifted = false;
  return true;
}

bool process_custom_shift_keys(uint16_t keycode, keyrecord_t *record) {
  static uint16_t registered_keycode = KC_NO;

  // If a custom shift key is registered, then this event is either
  // releasing it or manipulating another key at the same time. Either way,
  // we release the currently registered key.
  if (registered_keycode != KC_NO) {
    unregister_code16(registered_keycode);
    registered_keycode = KC_NO;
  }

  // Search for a custom key with keycode equal to `keycode`.
  for (int i = 0; i < NUM_CUSTOM_SHIFT_KEYS; ++i) {
    if (keycode == custom_shift_keys[i].keycode) {
      if (record->event.pressed) {
        const uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
        if ((mods | get_oneshot_mods()) & MOD_MASK_SHIFT) {
          del_oneshot_mods(MOD_MASK_SHIFT);
#else
        if (mods & MOD_MASK_SHIFT) {
#endif  // NO_ACTION_ONESHOT
          del_mods(MOD_MASK_SHIFT);
          registered_keycode = custom_shift_keys[i].shifted_keycode;
        } else {
          registered_keycode = custom_shift_keys[i].keycode;
        }

        register_code16(registered_keycode);
        set_mods(mods);  // Restore the mods.
      }

      return false;
    }
  }

  return true;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}


void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
        // Must come before L_NUM as it's an overridelayer
        if (IS_LAYER_ON(L_MOUS)) {
            oled_write_ln_P(PSTR("Mouse"), false);
            return;
        }
        if (IS_LAYER_ON(L_NAV)) {
            oled_write_ln_P(PSTR("Navigation"), false);
            return;
        }
        if (IS_LAYER_ON(L_SYM)) {
            oled_write_ln_P(PSTR("Symbol"), false);
            return;
        }
        // Must come before L_NUM as it's a one shot layer
        if (IS_LAYER_ON(L_FUN)) {
            oled_write_ln_P(PSTR("Function"), false);
            return;
        }
        if (IS_LAYER_ON(L_NUM)) {
            oled_write_ln_P(PSTR("Numpad"), false);
            return;
        }
        if (IS_LAYER_ON(L_MED)) {
            oled_write_ln_P(PSTR("Media"), false);
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
