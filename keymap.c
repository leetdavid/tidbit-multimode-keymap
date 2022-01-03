/* Copyright 2021 Chris Tanaka <https://github.com/christanaka>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Displays various statuses and a cute pet on your OLED.
 * This is a modified port of HellSingCoder's adorable keyboard pet:
 * <https://github.com/HellSingCoder/qmk_firmware/tree/master/keyboards/sofle/keymaps/helltm>
 * VIA compatible.
 *
 * Statuses:
 * - WPM Counter
 * - Top Layer
 * - Caps Lock Indicator
 *
 * Pet Features:
 * - WPM <10: Sit
 * - WPM >=10 and <=40: Walk
 * - WPM >40: Run
 * - Caps Lock: Bark
 * - Ctrl: Sneak
 * - Space: Jump
 */

#ifdef OLED_ENABLE
  #include "pet.h"
  #include "status.h"
#endif
#include QMK_KEYBOARD_H

// enum layer_number = {
//   _BASE = 0,
//   _VIA1 = 1,
//   _VIA2 = 2,
//   _TOG0 = 3,
//   _TOG1 = 4,
//   _TOG2 = 5,
// };

#define _BASE     0
#define _VIA1     1
#define _VIA2     2

#define _TOG0     3
#define _TOG1     4
#define _TOG2     5

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // ==========================================
  // ======           1st Mode           ======
  // ==========================================
  [_BASE] = LAYOUT(
    //-------,----------,----------,----------,
                   KC_NO, TO(_VIA1), TO(_VIA2),
    //-------,----------,----------,----------,
        RESET,   KC_KP_7,   KC_KP_8,   KC_KP_9,
    //-------,----------,----------,----------,
      KC_PAST,   KC_KP_4,   KC_KP_5,   KC_KP_6,
    //-------,----------,----------,----------,
      KC_PMNS,   KC_KP_1,   KC_KP_2,   KC_KP_3,
    //-------,----------,----------,----------,
    MO(_TOG0),   KC_KP_0,    KC_DOT,    KC_ENT
    //-------,----------,----------,----------,
  ),
  [_VIA1] = LAYOUT(
    //-------,----------,----------,----------,
               TO(_BASE),     KC_NO, TO(_VIA2),
    //-------,----------,----------,----------,
        KC_NO,      KC_Q,      KC_W,      KC_E,
    //-------,----------,----------,----------,
       KC_TAB,      KC_A,      KC_S,      KC_D,
    //-------,----------,----------,----------,
      KC_LSFT,      KC_Z,      KC_X,      KC_C,
    //-------,----------,----------,----------,
      KC_LCTL,   KC_LGUI, MO(_TOG1),    KC_SPC
    //-------,----------,----------,----------,
  ),
  [_VIA2] = LAYOUT(
    //-------,----------,----------,----------,
               TO(_BASE), TO(_VIA1),     KC_NO,
    //-------,----------,----------,----------,
         KC_A,      KC_A,      KC_A,      KC_A,
    //-------,----------,----------,----------,
         KC_A,      KC_A,      KC_A,      KC_A,
    //-------,----------,----------,----------,
         KC_A,      KC_A,      KC_A,      KC_A,
    //-------,----------,----------,----------,
    MO(_TOG2),      KC_A,      KC_A,      KC_A
    //-------,----------,----------,----------,
  ),


  [_TOG0] = LAYOUT(
    //-------,----------,----------,----------,
                   KC_NO, TO(_VIA1), TO(_VIA2),
    //-------,----------,----------,----------,
      KC_MPLY,   KC_TRNS,     KC_UP,   KC_TRNS,
    //-------,----------,----------,----------,
      KC_PSLS,   KC_LEFT,   KC_DOWN,   KC_RGHT,
    //-------,----------,----------,----------,
      KC_PPLS,     RESET,   EEP_RST,     DEBUG,
    //-------,----------,----------,----------,
      KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_PEQL
    //-------,----------,----------,----------,
  ),
  [_TOG1] = LAYOUT(
    //-------,----------,----------,----------,
               TO(_BASE),     KC_NO, TO(_VIA2),
    //-------,----------,----------,----------,
        KC_NO,   KC_TRNS,     KC_UP,      KC_R,
    //-------,----------,----------,----------,
      KC_TRNS,   KC_LEFT,   KC_DOWN,   KC_RGHT,
    //-------,----------,----------,----------,
      KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,
    //-------,----------,----------,----------,
      KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS
    //-------,----------,----------,----------,
  ),
  [_TOG2] = LAYOUT(
    //-------,----------,----------,----------,
               TO(_BASE), TO(_VIA1),     KC_NO,
    //-------,----------,----------,----------,
        KC_NO,   RGB_TOG,   RGB_MOD,  RGB_RMOD,
    //-------,----------,----------,----------,
      RGB_VAI,   RGB_HUI,   RGB_SAI,   RGB_SPI,
    //-------,----------,----------,----------,
      RGB_VAD,   RGB_HUD,   RGB_SAD,   RGB_SPD,
    //-------,----------,----------,----------,
      KC_TRNS,     RESET,   EEP_RST,     DEBUG
    //-------,----------,----------,----------,
  )
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

bool oled_task_user(void) {
  status_render_layer(0, 0);

  // status_render_wpm(0, 0);
  // status_render_caps_lock(0, 7);

  pet_render(0, 13);

  return true;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  process_record_remote_kb(keycode, record);
  pet_process_record(keycode, record);

  switch(keycode) {
    case DF(_BASE):
      if (record -> event.pressed) {
        set_single_persistent_default_layer(_BASE);
        return false;
      }
    case DF(_VIA1):
      if (record -> event.pressed){
        set_single_persistent_default_layer(_VIA1);
        return false;
      }
    case DF(_VIA2):
      if (record -> event.pressed){
        set_single_persistent_default_layer(_VIA2);
        return false;
      }
  }

  return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
  if (clockwise) {
    tap_code(KC_VOLU);
  } else {
    tap_code(KC_VOLD);
  } 
  return true;
}

bool wpm_keycode_user(uint16_t keycode) {
  if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
      (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) ||
      (keycode >= QK_MODS && keycode <= QK_MODS_MAX)) {
    keycode = keycode & 0xFF;
  } else if (keycode > 0xFF) {
    keycode = 0;
  }

  // Include keys in WPM calculation
  if ((keycode >= KC_A && keycode <= KC_0) || // Alphas - Numbers
      (keycode >= KC_TAB && keycode <= KC_SLASH) || // Tab - Slash (Symbols, Punctuation, Space)
      (keycode >= KC_KP_1 && keycode <= KC_KP_DOT) ||  // Keypad numbers - Keypad Dot
      (keycode >= KC_F1 && keycode <= KC_F12)) { // F1 - F12
    return true;
  }

  return false;
}
