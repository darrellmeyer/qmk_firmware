#include QMK_KEYBOARD_H
#include "quantum.h"
#include "action.h"


// ` 9 0 - = bspc
// tab O P [ ] bsls
// caps L ; '
// V B N M , . /

enum combos {
  RBRC,
  N1_3,
  Q_E,
  Q_2_E,
  E_2,
  Q_2,
  E_RBRC,
  SLSH_E,
};
const uint16_t PROGMEM rbrc_combo[] = {KC_RBRC, COMBO_END};
const uint16_t PROGMEM n13_combo[] = {KC_1, KC_3, COMBO_END};
// const uint16_t PROGMEM n23_combo[] = {KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM qe_combo[] = {KC_Q, KC_E, COMBO_END};
const uint16_t PROGMEM q2e_combo[] = {KC_Q, KC_2, KC_E, COMBO_END};
const uint16_t PROGMEM q2_combo[] = {KC_Q, KC_2, COMBO_END};
const uint16_t PROGMEM e2_combo[] = {KC_2, KC_E, COMBO_END};
// const uint16_t PROGMEM je_combo[] = {KC_J, KC_E, COMBO_END};
// const uint16_t PROGMEM jq_combo[] = {KC_J, KC_Q, COMBO_END};
// const uint16_t PROGMEM q_rbrc_combo[] = {KC_Q, KC_RBRC, COMBO_END};
const uint16_t PROGMEM e_rbrc_combo[] = {KC_E, KC_RBRC, COMBO_END};


combo_t key_combos[] = {
//   [RBRC] = COMBO(rbrc_combo, KC_TAB),
  [N1_3] = COMBO(n13_combo, KC_F2),
//   [N2_3] = COMBO(n23_combo, KC_P),
//   [Q_E] = COMBO(qe_combo, KC_ESC), // mount
//   [Q_2_E] = COMBO(q2e_combo, KC_ESC),
//   [E_2] = COMBO(e2_combo, C(A(S(KC_BSLS)))), // game mute
//   [Q_2] = COMBO(q2_combo, C(A(S(KC_GRV)))), // mic mute
//   [J_E] = COMBO(je_combo, KC_O),
//   [J_Q] = COMBO(jq_combo, KC_O),w
//   [Q_RBRC] = COMBO(q_rbrc_combo, KC_O),
//   [E_RBRC] = COMBO(e_rbrc_combo, KC_O),

};

enum td_keycodes {
  Q_O, // mount
  R_TAB, // plates
  Z_P, // ping / danger ping
//   F_P   ,]
//   Z_SCLN, // map scoreboard/backpack
//   X_ESC2,
  RBRC_M, // ping/danger ping
  K1_ESC,
  K2_MUTE_APP,
  K3_MUTE_MIC,
  K4_SETTINGS,
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(Q_O):
        case TD(R_TAB):
        case TD(Z_P):
        // case TD(F_P):
        // case TD(Z_SCLN):
        // case TD(X_ESC2):
        case TD(RBRC_M):
        case TD(K1_ESC):
        case TD(K2_MUTE_APP):
        case TD(K3_MUTE_MIC):
        case TD(K4_SETTINGS):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
    }
    return true;
}

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

tap_dance_action_t tap_dance_actions[] = {
    [Q_O] = ACTION_TAP_DANCE_TAP_HOLD(KC_Q, KC_O),
    [R_TAB] = ACTION_TAP_DANCE_TAP_HOLD(KC_R, KC_TAB),
    [Z_P] = ACTION_TAP_DANCE_TAP_HOLD(KC_Z, KC_P),
    // [R_COMM] = ACTION_TAP_DANCE_TAP_HOLD(KC_R, KC_COMM),
    // [F_P] = ACTION_TAP_DANCE_TAP_HOLD(KC_F, KC_P),
    // [Z_SCLN] = ACTION_TAP_DANCE_TAP_HOLD(KC_Z, KC_SCLN),

    [RBRC_M] = ACTION_TAP_DANCE_TAP_HOLD(KC_RBRC, KC_M),
    [K1_ESC] = ACTION_TAP_DANCE_TAP_HOLD(KC_1, KC_ESC),
    [K2_MUTE_APP] = ACTION_TAP_DANCE_TAP_HOLD(KC_2, KC_F13),
    [K3_MUTE_MIC] = ACTION_TAP_DANCE_TAP_HOLD(KC_3, KC_F14),
    [K4_SETTINGS] = ACTION_TAP_DANCE_TAP_HOLD(KC_4, KC_F2),
    // [X_ESC2] = ACTION_TAP_DANCE_TAP_HOLD(KC_X, KC_ESC),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [0] = LAYOUT_ortho_5x12(
        QK_BOOT,   KC_F1,            KC_NO,KC_NO,       KC_8,       KC_7,       TD(K1_ESC), TD(K2_MUTE_APP), TD(K3_MUTE_MIC), TD(K4_SETTINGS),  KC_5,      KC_6,
        KC_F7,     KC_F2,            KC_NO,KC_NO,       KC_NO,      KC_U,       TD(Q_O),    KC_W,            KC_E,            TD(R_TAB),        KC_T,      KC_Y,
        A(KC_Z),   KC_F3,            KC_NO,KC_NO,       MO(1),      KC_J,       KC_A,       KC_S,            KC_D,            KC_F,             KC_G,      KC_H,
        A(KC_F9),  C(A(S(KC_BSLS))), KC_NO,KC_NO,       KC_ESC,     KC_LCTL,    TD(Z_P),    KC_X,            KC_C,            KC_LBRC,          KC_SPC,    KC_NO,
        A(KC_F10), C(A(S(KC_GRV))),  KC_NO,KC_NO,       KC_APP,     OSL(1),     KC_LALT,    KC_NO,           KC_BSLS,         KC_NO,            KC_NO,     TD(RBRC_M)
        ),
        [1] = LAYOUT_ortho_5x12(
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,    C(A(S(KC_GRV))), C(A(S(KC_BSLS))), KC_TRNS,  KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_T,       KC_TRNS,         KC_T,             KC_TRNS,  KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_J,       KC_TRNS,    KC_TRNS,         KC_R,             KC_TRNS,  KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_TRNS,          KC_TRNS,  KC_TAB,    KC_TRNS,
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_TRNS,          KC_TRNS,  KC_TRNS,   KC_TRNS
        ),
        [2] = LAYOUT_ortho_5x12(
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_TRNS,          KC_TRNS,  KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_TRNS,          KC_TRNS,  KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_Y,       KC_TRNS,         KC_QUOT,          KC_TRNS,  KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_M,       KC_TRNS,    KC_TRNS,         KC_TRNS,          KC_TRNS,  KC_TAB,    KC_TRNS,
        KC_TRNS,   KC_TRNS,          KC_TRNS,KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,         KC_TRNS,          KC_TRNS,  KC_TRNS,   KC_TRNS
        )
};

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
