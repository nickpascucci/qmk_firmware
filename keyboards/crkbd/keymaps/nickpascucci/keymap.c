#include QMK_KEYBOARD_H
#include <print.h>

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

#define THM_4 LT(L_MED,   KC_ENT)
#define THM_5 LT(L_SYM,   KC_SPC)
#define THM_6 KC_MINS

// Redefine copy/paste for Mac
#define KC_COPY LGUI(KC_C)
#define KC_PSTE LGUI(KC_V)
#define KC_CUT  LGUI(KC_X)

// Browser navigation
#define KC_BCKH LGUI(KC_LBRC)
#define KC_FWDH LGUI(KC_RBRC)

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
  {KC_LPRN, KC_RPRN}, // "Shift + (" gives ")"
  {KC_LBRC, KC_RBRC}, // "Shift + [" gives "]"
  {KC_LCBR, KC_RCBR}, // "Shift + {" gives "}"
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

// Combos allow access to common symbols without shifting layers.
//
// In selecting keys to use as combos, it's best to avoid using the pinkies where possible - because
// they are so much weaker than the other fingers it makes it hard to hit both keys at once.
//
// Similarly, avoiding the home row is wise, especially on Colemak, because the inward rolls often
// trigger macros accidentally.

const uint16_t PROGMEM combo_wf[] = {KC_W, KC_F, COMBO_END};
const uint16_t PROGMEM combo_fp[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM combo_pb[] = {KC_P, KC_B, COMBO_END};

const uint16_t PROGMEM combo_xc[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_cd[] = {KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM combo_dv[] = {KC_D, KC_V, COMBO_END};

const uint16_t PROGMEM combo_jl[] = {KC_J, KC_L, COMBO_END};
const uint16_t PROGMEM combo_lu[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM combo_uy[] = {KC_U, KC_Y, COMBO_END};

// Symbols use uppercase because we need ASCII identifiers
const uint16_t PROGMEM combo_AH[] = {KC_AT, KC_HASH, COMBO_END};
const uint16_t PROGMEM combo_HB[] = {KC_HASH, KC_LBRC, COMBO_END};
const uint16_t PROGMEM combo_BP[] = {KC_LBRC, KC_PERC, COMBO_END};

const uint16_t PROGMEM combo_AA[] = {KC_AMPR, KC_ASTR, COMBO_END};
const uint16_t PROGMEM combo_AP[] = {KC_ASTR, KC_LPRN, COMBO_END};
const uint16_t PROGMEM combo_PD[] = {KC_LPRN, KC_DLR, COMBO_END};

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
    KC_TIME, // print key timings to debug output
    KC_SPCL, // go to the next space to the left
    KC_SPCR, // go to the next space to the right
    KC_MSNC, // open mission control
    KC_EXPS, // open expose view
};

// Note: If you add or remove elements from this array, update COMBO_COUNT in config.h
combo_t key_combos[COMBO_COUNT] = {
    // Bottom row: quick cut, copy, and paste
    COMBO(combo_xc, KC_CUT),
    COMBO(combo_cd, KC_COPY),
    COMBO(combo_dv, KC_PSTE),

    // Symbol rows: programming bigrams
    COMBO(combo_AH, KC_DPIP),
    COMBO(combo_HB, LCTL(KC_6)), // Vim: change to alternate buffer
    COMBO(combo_BP, KC_DAMP),

    COMBO(combo_AA, KC_DRRW),
    COMBO(combo_AP, KC_DCLN),
    COMBO(combo_PD, KC_ARRW),

    // Caps Word
    COMBO(combo_tn, KC_CAPW),
};

// Array containing the aggregate timing of keystrokes since last restart.
// First element is the total time spent holding the key. Second is the number of presses. Divide
// top by bottom to get the average hold time.
typedef struct {
    uint32_t time;
    uint32_t presses;
} keytiming_t;

#ifdef CONSOLE_ENABLE
static keytiming_t key_timings[MATRIX_ROWS][MATRIX_COLS];

// Tracks when the current key was first pressed; see record_key_timings.
static uint16_t key_down_time = 0;
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [L_COLE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                         KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,   MA(A),   MG(R),   MC(S),   MS(T),   MH(G),                        MH(M),   MS(N),   MC(E),   MG(I),   MA(O), XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                         KC_K,    KC_H, KC_COMM,  KC_DOT, KC_SLSH, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            THM_1,   THM_2,   THM_3,      THM_4,   THM_5,   THM_6
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_SYM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, KC_EXLM,   KC_AT, KC_HASH, KC_LBRC, KC_PERC,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN,  KC_DLR,                      KC_HYPR, KC_RSFT, KC_RCTL, KC_RGUI, KC_RALT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_GRV, KC_BSLS,  KC_PIPE, KC_LCBR, KC_TILD,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_EQL, KC_QUOT, KC_MINS,    XXXXXXX, XXXXXXX, XXXXXXX
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_NUM] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_TIME,                      KC_PLUS,    KC_7,    KC_8,    KC_9, KC_ASTR, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LALT, KC_LGUI, KC_LCTL, KC_LSFT, KC_HYPR,                       KC_EQL,    KC_4,    KC_5,    KC_6,    KC_0, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,    KC_1,    KC_2,    KC_3, KC_SLSH, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, OSL(L_FUN)
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_NAV] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, KC_SPCL, KC_MSNC, KC_EXPS, KC_SPCR, XXXXXXX,                      KC_HOME, KC_PSTE, KC_COPY,  KC_CUT, KC_UNDO, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_LALT, KC_LGUI, KC_LCTL, KC_LSFT, KC_HYPR,                      KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_AGIN, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_END, KC_PGDN, KC_PGUP, KC_BCKH, KC_FWDH, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,    KC_SPCL, KC_SPCR, TO(L_MOUS)
                                      //|--------+--------+--------|  |--------+--------+--------|
  ),

  [L_MOUS] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, KC_ACL2, KC_WH_D, KC_MS_U, KC_WH_U, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_ACL1, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,                      KC_HYPR, KC_RSFT, KC_RCTL, KC_RGUI, KC_RALT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, KC_ACL0, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_BTN3, KC_BTN1, KC_BTN2,    XXXXXXX, XXXXXXX, TO(L_COLE)
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

void keyboard_post_init_user(void) {
  debug_enable = true;
}

void record_key_timings(keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
  if (IS_PRESSED(record->event) && key_down_time == 0) {
    // Remember when the key was pressed, so we can compute the hold time later.
    key_down_time = timer_read32();
  } else if (IS_RELEASED(record->event) && key_down_time != 0) {
    // On release, if we are tracking a key hold, update the corresponding timing record.
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;

    keytiming_t *timing = &key_timings[row][col];

    timing->time = timing->time + timer_elapsed32(key_down_time);
    timing->presses = timing->presses + 1;

    uprintf("%u,%u,", row, col);
    uprintf("%u,", timing->time);
    uprintf("%u\r\n", timing->presses);

    key_down_time = 0;
  }
#endif
}

// Print timing data to the debug output
void dump_timings(void) {
#ifdef CONSOLE_ENABLE
  uprintf("=== Timing Info ===\r\n");
  uprintf("row,col,time,presses,avg\r\n");
  keytiming_t *timing;
  for (int row = 0; row < MATRIX_ROWS; row++) {
    for (int col = 0; col < MATRIX_COLS; col++) {
      timing = &key_timings[row][col];
      uint32_t avg_time = timing->time / timing->presses;
      // If we have no data, don't print out 0/0
      if (timing->presses == 0) {
        avg_time = 0;
      }
      // Formatting a u32 is a lot of work, so do one per line. Otherwise this just prints out '0'.
      uprintf("%u,%u,", row, col);
      uprintf("%u,", timing->time);
      uprintf("%u,", timing->presses);
      uprintf("%u\r\n", avg_time);
    }
  }
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  record_key_timings(record);

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

    case KC_TIME:
      if (record->event.pressed) {
        dump_timings();
      }
      return false;

    case KC_SPCL:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_TAP(X_LEFT)));
      }
      return false;

    case KC_SPCR:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_TAP(X_RIGHT)));
      }
      return false;

    case KC_MSNC:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_TAP(X_UP)));
      }
      return false;

    case KC_EXPS:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL(SS_TAP(X_DOWN)));
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
      // Letter keys and - should be shifted.
      case KC_A ... KC_Z:
      case KC_MINS:
      case KC_UNDS:
        if (!shifted) { register_code(KC_LSFT); }
        shifted = true;
        return true;

      // Keycodes that continue caps word but shouldn't get shifted.
      case KC_1 ... KC_0:
      case KC_BSPC:
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

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, !IS_LAYER_ON(L_COLE));
}

void oled_render_layer_state(void) {
        // Must come before L_NUM as it's an overridelayer
        if (IS_LAYER_ON(L_MOUS)) {
            oled_write_ln_P(PSTR("MOUSE LOCK"), false);
            return;
        }
        if (IS_LAYER_ON(L_NAV)) {
            oled_write_ln_P(PSTR("NAVIGATION"), false);
            return;
        }
        if (IS_LAYER_ON(L_SYM)) {
            oled_write_ln_P(PSTR("SYMBOL"), false);
            return;
        }
        // Must come before L_NUM as it's a one shot layer
        if (IS_LAYER_ON(L_FUN)) {
            oled_write_ln_P(PSTR("FN ONE-SHOT"), false);
            return;
        }
        if (IS_LAYER_ON(L_NUM)) {
            oled_write_ln_P(PSTR("NUMPAD"), false);
            return;
        }
        if (IS_LAYER_ON(L_MED)) {
            oled_write_ln_P(PSTR("MEDIA"), false);
            return;
        }
        // Note: these should be last so that if other layers are active they get priority.
        if (IS_LAYER_ON(L_COLE)) {
            oled_write_ln_P(PSTR("COLEMAK"), false);
            return;
        }
}

void oled_render_mod_state(void) {
        const uint8_t mods = get_mods() | get_oneshot_mods();
        oled_write_P(PSTR("SFT"), mods & MOD_MASK_SHIFT);
        oled_write_P(PSTR(" "), false);
        oled_write_P(PSTR("CTL"), mods & MOD_MASK_CTRL);
        oled_write_P(PSTR(" "), false);
        oled_write_P(PSTR("GUI"), mods & MOD_MASK_GUI);
        oled_write_P(PSTR(" "), false);
        oled_write_P(PSTR("ALT"), mods & MOD_MASK_ALT);
        oled_write_P(PSTR(" "), false);
        oled_write_ln_P(PSTR("CAPS"), caps_word_enabled);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_mod_state();
    } else {
        oled_render_logo();
    }
    return false;
}
#endif // OLED_ENABLE
