#include QMK_KEYBOARD_H
#include "process_combo.h"
#include "process_tap_dance.h"

enum combos {
//   Q_E,
//   Q_E_SPC,

//   Q_SPC,
//   E_SPC,
//   R_SPC,
  TAB_SPC,
};

// Q : next weapon
// Q_SPC : ??

// 3 : ping danger
// E_SPC : ping

// R : reload
// R:hld stow

// 1 : walk
// 2 : auto forward

// H : Tab
// H+SPC : Warzone score


// const uint16_t PROGMEM qe_combo[] = {KC_Q, KC_E, COMBO_END};
// const uint16_t PROGMEM qe_spc_combo[] = {KC_Q, KC_E, KC_SPC, COMBO_END};

// const uint16_t PROGMEM q_spc_combo[] = {KC_Q, KC_SPC, COMBO_END};
// const uint16_t PROGMEM e_spc_combo[] = {KC_E, KC_SPC, COMBO_END};
// const uint16_t PROGMEM r_spc_combo[] = {KC_R, KC_SPC, COMBO_END};
// const uint16_t PROGMEM tab_spc_combo[] = {KC_TAB, KC_SPC, COMBO_END};


// combo_t key_combos[] = {
//   [Q_E] = COMBO(qe_combo, KC_M),
//   [Q_E_SPC] = COMBO(qe_spc_combo, KC_SCLN),

//   [Q_SPC] = COMBO(q_spc_combo, KC_U),
//   [E_SPC] = COMBO(e_spc_combo, KC_P),
//   [R_SPC] = COMBO(r_spc_combo, KC_O),
//   [TAB_SPC] = COMBO(tab_spc_combo, KC_SCLN),
// };




// uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case LT(1, KC_SPC):
//             return TAPPING_TERM + 100;
//         default:
//             return TAPPING_TERM;
//     }
// }



// typedef enum {
//     TD_NONE,
//     TD_UNKNOWN,
//     TD_SINGLE_TAP,
//     TD_SINGLE_HOLD,
//     TD_DOUBLE_TAP,
//     TD_DOUBLE_HOLD,
//     TD_DOUBLE_SINGLE_TAP, // Send two single taps
//     TD_TRIPLE_TAP,
//     TD_TRIPLE_HOLD
// } td_state_t;

// typedef struct {
//     bool is_press_action;
//     td_state_t state;
// } td_tap_t;

// td_state_t cur_dance(tap_dance_state_t *state);

// For the q tap dance. Put it here so it can be used in any keymap
// void tab_finished(tap_dance_state_t *state, void *user_data);
// void tab_reset(tap_dance_state_t *state, void *user_data);

// td_state_t cur_dance(tap_dance_state_t *state) {
//     if (state->count == 1) {
//         if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
//         // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
//         else return TD_SINGLE_HOLD;
//     } else if (state->count == 2) {
//         // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
//         // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
//         // keystrokes of the key, and not the 'double tap' action/macro.
//         if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
//         else if (state->pressed) return TD_DOUBLE_HOLD;
//         else return TD_DOUBLE_TAP;
//     }

//     // Assumes no one is trying to type the same letter three times (at least not quickly).
//     // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
//     // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
//     if (state->count == 3) {
//         if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
//         else return TD_TRIPLE_HOLD;
//     } else return TD_UNKNOWN;
// }

// // Create an instance of 'td_tap_t' for the 'x' tap dance.
// static td_tap_t xtap_state = {
//     .is_press_action = true,
//     .state = TD_NONE
// };

// void tab_finished(tap_dance_state_t *state, void *user_data) {
//     xtap_state.state = cur_dance(state);
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP: register_code(KC_TAB); break;
//         case TD_SINGLE_HOLD: register_code(KC_M); break;
//         // case TD_DOUBLE_TAP: register_code(KC_ESC); break;
//         case TD_DOUBLE_HOLD: register_code(KC_TAB); break;
//         // Last case is for fast typing. Assuming your key is `f`:
//         // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
//         // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
//         // case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
//         default: break;
//     }
// }

// void tab_reset(tap_dance_state_t *state, void *user_data) {
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP: unregister_code(KC_TAB); break;
//         case TD_SINGLE_HOLD: unregister_code(KC_M); break;
//         // case TD_DOUBLE_TAP: unregister_code(KC_ESC); break;
//         case TD_DOUBLE_HOLD: unregister_code(KC_TAB); break;
//         // case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
//         default: break;
//     }
//     xtap_state.state = TD_NONE;
// }

// void e_finished(tap_dance_state_t *state, void *user_data);
// void e_reset(tap_dance_state_t *state, void *user_data);

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;
    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }


enum td_keycodes {
  E_STOW,
  Q_Y,
};

const int MUTE_MIC = KC_F14;
const int MUTE_APP = KC_F13;
const int MUTE_CHAT = KC_F15;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(Q_Y):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    }
    return true;
}

tap_dance_action_t tap_dance_actions[] = {
    [E_STOW] = ACTION_TAP_DANCE_DOUBLE(KC_E, KC_O),
    [Q_Y] = ACTION_TAP_DANCE_TAP_HOLD(KC_Q, KC_Y),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_preonic_grid(
            KC_F1,     KC_F6,          KC_NO,  KC_NO,KC_NO,         KC_GRAVE,      KC_1,         KC_2,           KC_3,         KC_4,         KC_5,      KC_NO,
            KC_F2,     KC_F7,          KC_NO,  KC_NO,KC_NO,         KC_TAB,        TD(Q_Y),      KC_W,           TD(E_STOW),   KC_R,         KC_T,      KC_NO,
            KC_F3,     KC_F8,          KC_NO,  KC_NO,KC_NO,         KC_H,          KC_A,         KC_S,           KC_D,         KC_F,         KC_G,      KC_NO,
            KC_F4,     KC_F9,          KC_NO,  KC_NO,KC_NO,         KC_UP,         KC_Z,         KC_X,           KC_C,         KC_V,         KC_NO,     KC_NO,
            KC_F5,     KC_F10,         KC_NO,  KC_NO,KC_NO,         KC_NO,         KC_LALT,      KC_END,         KC_B,         KC_LBRC,      KC_SPC,    LT(1, KC_ESC)
    ),
    [1] = LAYOUT_preonic_grid(
            QK_BOOT,   KC_TRNS,        KC_TRNS,KC_TRNS,KC_TRNS,     KC_TRNS,       KC_6,         KC_7,           KC_8,         KC_9,         KC_0,      KC_TRNS,
            KC_TRNS,   KC_TRNS,        KC_TRNS,KC_TRNS,KC_TRNS,     KC_TRNS,       MUTE_MIC,     KC_TRNS,        MUTE_APP,     MUTE_CHAT,    KC_TRNS,   KC_TRNS,
            KC_TRNS,   KC_TRNS,        KC_TRNS,KC_TRNS,KC_TRNS,     KC_TRNS,       KC_TRNS,      KC_TRNS,        KC_F3,        KC_TRNS,      KC_TRNS,   KC_TRNS,
            KC_TRNS,   KC_TRNS,        KC_TRNS,KC_TRNS,KC_TRNS,     KC_TRNS,       A(KC_F9),     KC_TRNS,        A(KC_F10),    KC_TRNS,      KC_TRNS,   KC_TRNS,
            KC_TRNS,   KC_TRNS,        KC_TRNS,KC_TRNS,KC_TRNS,     KC_TRNS,       KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,      KC_TRNS,   KC_TRNS
    )
};

