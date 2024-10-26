#include QMK_KEYBOARD_H
#include "quantum_keycodes.h"
#include "action.h"
#include "keycodes.h"
#include "process_tap_dance.h"
#include "process_combo.h"

const uint16_t PROGMEM qe_combo[] = {KC_Q, KC_E, COMBO_END};
const uint16_t PROGMEM q_spc_combo[] = {KC_Q, KC_SPC, COMBO_END};
const uint16_t PROGMEM e_spc_combo[] = {KC_E, KC_SPC, COMBO_END};

combo_t key_combos[] = {
    COMBO(qe_combo, KC_I),
    COMBO(q_spc_combo, KC_O),
    COMBO(e_spc_combo, KC_N),
};


const int MUTE_MIC = KC_F14;
const int MUTE_APP = KC_F13;
const int MUTE_CHAT = KC_F15;

const int PLAY = KC_MEDIA_PLAY_PAUSE;
const int NEXT = KC_MEDIA_NEXT_TRACK;

const int SP_TOG = A(KC_F3);
const int SP_REC = A(KC_F9);
const int SP_LAST = A(KC_F10);

const int CROUCH = KC_Z;
const int PRONE = KC_C;

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
  K4_P,
  K5_B,
  SLIDE = SAFE_RANGE
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(K4_P):
        case TD(K5_B):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
            break;
        case SLIDE:
          if (record->event.pressed) {
            tap_code16(CROUCH);
          } else {
            tap_code16(KC_SPC);
          }
    }
    return true;
}

tap_dance_action_t tap_dance_actions[] = {
    [K4_P] = ACTION_TAP_DANCE_TAP_HOLD(KC_4, KC_P),
    [K5_B] = ACTION_TAP_DANCE_TAP_HOLD(KC_5, KC_B),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_preonic_grid(
            KC_F1,   KC_F6,       KC_NO,KC_NO,         KC_GRV,       KC_1,         KC_2,         KC_3,       TD(K4_P),      TD(K5_B),   KC_MINS,     KC_NO,
            KC_F2,   KC_F7,       KC_NO,KC_NO,         KC_U,         KC_HOME,      RCS(KC_LBRC), C(KC_SLSH), RCS(KC_RBRC),  KC_END,     KC_T,        KC_NO,
            KC_F3,   KC_F8,       KC_NO,KC_NO,         KC_SEMICOLON, KC_COLN,      LSFT(KC_J),   KC_SPC,     S(KC_L),       KC_QUOT,    KC_NO,       KC_NO,
            KC_F4,   KC_F9,       KC_NO,KC_NO,         KC_ESC,       KC_A,         KC_B,         KC_X,       C(KC_SLSH),    C(KC_SLSH), KC_NO,       KC_NO,
            KC_F5,   KC_F10,      KC_NO,KC_NO,         KC_LALT,      KC_LCTL,      KC_TAB,       KC_NO,      KC_SLSH,       KC_RSFT,    KC_SPC,      KC_RCTL
    ),
    [1] = LAYOUT_preonic_grid(
            QK_BOOT, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_6,      KC_7,      KC_8,       KC_9,      KC_0,      KC_TRNS,     KC_TRNS,
            KC_TRNS, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_TRNS,   KC_I,      KC_O,       KC_N,      KC_F3,     KC_TRNS,     KC_TRNS,
            KC_TRNS, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_TRNS,   KC_V,      KC_K,       KC_L,      KC_TRNS,   KC_TRNS,     KC_TRNS,
            KC_TRNS, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_M,      SP_LAST,   MUTE_MIC,   PLAY,      KC_TRNS,   KC_TRNS,     KC_TRNS,
            KC_TRNS, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_TRNS,   SP_REC,    MUTE_APP,   NEXT,      KC_TRNS,   KC_TRNS,     KC_TRNS
    )
};

