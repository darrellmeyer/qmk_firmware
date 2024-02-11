#include "action.h"
#include "process_tap_dance.h"
#include "quantum.h"
#include QMK_KEYBOARD_H

enum combos {
  E_A,
  E_DOT,
  C_LSFT,
  A_D,
  DOT_C,

  E_SPC,
  Z_SPC,
  A_SPC,
  X_SPC2,
  D_SPC,
  W_SPC,
  Q_SPC,
  Q_E,
//   R_SPC,
  F_SPC,
  A_LBRC,
  D_LBRC,
  DOT_LBRC,
  DOT_SPC,
  A_D_SPC,
  DOT_D,
  DOT_A,
};

// const uint16_t PROGMEM ea_combo[] = {KC_E, KC_A, COMBO_END};
// const uint16_t PROGMEM e_dot_combo[] = {KC_E, KC_DOT, COMBO_END};
// const uint16_t PROGMEM e_spc_combo[] = {KC_E, KC_SPC, COMBO_END};
// const uint16_t PROGMEM z_spc_combo[] = {KC_Z, KC_SPC, COMBO_END};
// const uint16_t PROGMEM x_spc2_combo[] = {KC_X, KC_SPC, COMBO_END};
// const uint16_t PROGMEM c_lsft_combo[] = {KC_C, KC_LSFT, COMBO_END};
// const uint16_t PROGMEM ad_combo[] = {KC_A, KC_D, COMBO_END};
// const uint16_t PROGMEM dot_c_combo[] = {KC_DOT, KC_C, COMBO_END};
// const uint16_t PROGMEM a_spc_combo[] = {KC_A, KC_SPC, COMBO_END};
// const uint16_t PROGMEM d_spc_combo[] = {KC_D, KC_SPC, COMBO_END};
// const uint16_t PROGMEM w_spc_combo[] = {KC_W, KC_SPC, COMBO_END};
// const uint16_t PROGMEM q_spc_combo[] = {KC_Q, KC_SPC, COMBO_END};
// const uint16_t PROGMEM qe_combo[] = {KC_Q, KC_E, COMBO_END};
// // const uint16_t PROGMEM r_spc_combo[] = {KC_R, KC_SPC, COMBO_END};
// const uint16_t PROGMEM f_spc_combo[] = {KC_F, KC_SPC, COMBO_END};
// // const uint16_t PROGMEM d_lbrc_combo[] = {KC_D, KC_LBRC, COMBO_END};
// // const uint16_t PROGMEM a_lbrc_combo[] = {KC_A, KC_LBRC, COMBO_END};
// const uint16_t PROGMEM dot_lbrc_combo[] = {KC_DOT, KC_LBRC, COMBO_END};
// const uint16_t PROGMEM ad_spc_combo[] = {KC_A, KC_D, KC_SPC, COMBO_END};
// const uint16_t PROGMEM dot_spc_combo[] = {KC_DOT, KC_SPC, COMBO_END};
// const uint16_t PROGMEM dot_d_combo[] = {KC_DOT, KC_D, COMBO_END};
// const uint16_t PROGMEM dot_a_combo[] = {KC_DOT, KC_A, COMBO_END};
// o = mount
// p = plates
// l =
// t = tactical
// h = field upgrade
// z = crouch
// ; = danger ping
// : =
// m = map


// combo_t key_combos[] = {
//   [E_A] = COMBO(ea_combo, KC_P),
//   [E_DOT] = COMBO(e_dot_combo, KC_H),
//   [Q_SPC] = COMBO(q_spc_combo, KC_T),
//   [E_SPC] = COMBO(e_spc_combo, KC_L),
//   [R_SPC] = COMBO(r_spc_combo, KC_O),
//   [F_SPC] = COMBO(f_spc_combo, KC_H),
//   [X_SPC2] = COMBO(x_spc2_combo, KC_H),
//   [C_SPC] = COMBO(c_spc_combo, KC_H),
//   [Q_E] = COMBO(qe_combo, KC_ESC),

//   [Z_SPC] = COMBO(z_spc_combo, KC_M),

//   [C_LSFT] = COMBO(c_lsft_combo, KC_K),
//   [A_D] = COMBO(ad_combo, KC_SLSH),
//   [DOT_C] = COMBO(dot_c_combo, KC_M),
//   [A_SPC] = COMBO(a_spc_combo, KC_L),
//   [D_SPC] = COMBO(d_spc_combo, KC_T),
//   [DOT_SPC] = COMBO(dot_spc_combo, KC_TAB),
//   [W_SPC] = COMBO(w_spc_combo, KC_H),

//   [D_LBRC] = COMBO(d_lbrc_combo, KC_SCLN),
//   [A_LBRC] = COMBO(a_lbrc_combo, KC_Z),
//   [DOT_LBRC] = COMBO(dot_lbrc_combo, KC_ESC),
//   [DOT_D] = COtsMBO(dot_d_combo, KC_M),
//   [DOT_A] = COMBO(dot_a_combo, KC_ESC),
//   [A_D_SPC] = COMBO(ad_spc_combo, KC_Z),
// };

// \ : '
// L M O P

enum td_keycodes {
  Q_O, // mount
  R_P, // plates
  J_SCLN, // ping / danger ping
//   F_P   ,
//   Z_SCLN, // map scoreboard/backpack
//   X_ESC2,
  RBRC_QUOT, // ping/danger ping
  K1_F1,
  K2_F2,
  K3_F3
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
        case TD(R_P):
        case TD(J_SCLN):
        // case TD(F_P):
        // case TD(Z_SCLN):
        // case TD(X_ESC2):
        case TD(RBRC_QUOT):
        // case TD(K1_F1):
        // case TD(K2_F2):
        // case TD(K3_F3):
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
    [R_P] = ACTION_TAP_DANCE_TAP_HOLD(KC_R, KC_P),
    [J_SCLN] = ACTION_TAP_DANCE_TAP_HOLD(KC_J, KC_SCLN),
    // [R_COMM] = ACTION_TAP_DANCE_TAP_HOLD(KC_R, KC_COMM),
    // [F_P] = ACTION_TAP_DANCE_TAP_HOLD(KC_F, KC_P),
    // [Z_SCLN] = ACTION_TAP_DANCE_TAP_HOLD(KC_Z, KC_SCLN),

    [RBRC_QUOT] = ACTION_TAP_DANCE_TAP_HOLD(KC_RBRC, KC_QUOT),
    // [K1_F1] = ACTION_TAP_DANCE_DOUBLE(KC_1, KC_F1),
    // [K2_F2] = ACTION_TAP_DANCE_DOUBLE(KC_2, KC_F2),
    // [K3_F3] = ACTION_TAP_DANCE_DOUBLE(KC_3, KC_F3),

    // [X_ESC2] = ACTION_TAP_DANCE_TAP_HOLD(KC_X, KC_ESC),
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [0] = LAYOUT_ortho_5x12(
        KC_NO, KC_NO,   QK_BOOT,   KC_F1,            KC_8,   KC_7,       KC_1,       KC_2,       KC_3,       KC_4,     KC_5,   KC_6,
        KC_NO, KC_NO,   KC_F7,     KC_F2,            KC_I,   KC_U,       TD(Q_O),    KC_W,       KC_E,       TD(R_P),  KC_T,   KC_Y,
        KC_NO, KC_NO,   KC_F8,     KC_F3,            KC_K,   KC_J,       KC_A,       KC_S,       KC_D,       KC_F,     KC_G,   KC_H,
        KC_NO, KC_NO,   A(KC_F9),  C(A(S(KC_BSLS))), KC_M,   KC_LALT,    KC_Z,       KC_ESC,     KC_C,       KC_V,     KC_B,   KC_N,
        KC_NO, KC_NO,   A(KC_F10), C(A(S(KC_SLSH))), KC_APP, KC_LCTL,    KC_COMM,    KC_DOT,     KC_SLSH,    KC_LBRC,  KC_SPC, TD(RBRC_QUOT)
        )
};

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {

};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
