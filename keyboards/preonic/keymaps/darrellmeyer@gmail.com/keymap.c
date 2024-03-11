#include QMK_KEYBOARD_H
#include "quantum_keycodes.h"
#include "action.h"
#include "keycodes.h"
#include "process_tap_dance.h"


const int MUTE_MIC = KC_F14;
const int MUTE_APP = KC_F13;
const int MUTE_CHAT = KC_F15;

const int PLAY = KC_MEDIA_PLAY_PAUSE;
const int NEXT = KC_MEDIA_NEXT_TRACK;

const int SP_TOG = A(KC_F3);
const int SP_REC = A(KC_F9);
const int SP_LAST = A(KC_F10);

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
  Y_J,
  R_I,
  K1_6,
  K2_7,
  K3_8,
  K4_9,
  K5_0,
  SLIDE = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(Y_J):
        case TD(R_I):
        case TD(K1_6):
        case TD(K2_7):
        case TD(K3_8):
        case TD(K4_9):
        case TD(K5_0):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
            break;
        case SLIDE:
          if (record->event.pressed) {
            tap_code16(KC_Z);
          } else {
            tap_code16(KC_SPC);
          }
    }
    return true;
}

tap_dance_action_t tap_dance_actions[] = {
    [E_STOW] = ACTION_TAP_DANCE_DOUBLE(KC_E, KC_O),
    [Y_J] = ACTION_TAP_DANCE_TAP_HOLD(KC_Y, KC_J),
    [R_I] = ACTION_TAP_DANCE_TAP_HOLD(KC_R, KC_I),

    [K1_6] = ACTION_TAP_DANCE_TAP_HOLD(KC_1, KC_6),
    [K2_7] = ACTION_TAP_DANCE_TAP_HOLD(KC_2, KC_7),
    [K3_8] = ACTION_TAP_DANCE_TAP_HOLD(KC_3, KC_8),
    [K4_9] = ACTION_TAP_DANCE_TAP_HOLD(KC_4, KC_9),
    [K5_0] = ACTION_TAP_DANCE_TAP_HOLD(KC_5, KC_0),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_preonic_grid(
            KC_F1,     KC_F6,          KC_NO,KC_NO,            KC_GRV,       TD(K1_6),     TD(K2_7),     TD(K3_8),       TD(K4_9),      TD(K5_0),     KC_MINS,      KC_NO,
            KC_F2,     KC_F7,          KC_NO,KC_NO,            KC_NO,        TD(Y_J),      KC_Q,         KC_W,           TD(E_STOW),    TD(R_I),      KC_T,         KC_NO,
            KC_F3,     KC_F8,          KC_NO,KC_NO,            KC_TAB,       KC_LCTL,      KC_A,         KC_S,           KC_D,          KC_F,         KC_G,         KC_NO,
            KC_F4,     KC_F9,          KC_NO,KC_NO,            KC_ESC,       KC_LSFT,      KC_Z,         KC_X,           KC_C,          KC_V,         KC_NO,        KC_NO,
            KC_F5,     KC_F10,         KC_NO,KC_NO,            KC_LALT,      KC_TAB,       KC_TAB,       KC_DOT,         KC_SLSH,       SLIDE,        KC_SPC,       OSL(1)
    ),
    [1] = LAYOUT_preonic_grid(
            QK_BOOT,   KC_TRNS,        KC_TRNS,KC_TRNS,        KC_TRNS,      KC_6,         MUTE_CHAT,    MUTE_APP,       MUTE_MIC,      KC_TRNS,      KC_TRNS,      KC_TRNS,
            KC_TRNS,   KC_TRNS,        KC_TRNS,KC_TRNS,        KC_TRNS,      KC_TRNS,      PLAY,         KC_TRNS,        NEXT,          KC_TRNS,      KC_TRNS,      KC_TRNS,
            KC_TRNS,   KC_TRNS,        KC_TRNS,KC_TRNS,        KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,        KC_TRNS,       KC_F3,        KC_TRNS,      KC_TRNS,
            KC_TRNS,   KC_TRNS,        KC_TRNS,KC_TRNS,        KC_TRNS,      KC_TRNS,      KC_TRNS,      KC_TRNS,        KC_TRNS,       KC_TRNS,      KC_TRNS,      KC_TRNS,
            KC_TRNS,   KC_TRNS,        KC_TRNS,KC_TRNS,        KC_TRNS,      KC_TRNS,      SP_REC,       SP_TOG,         SP_LAST,       KC_TRNS,      KC_TRNS,      KC_TRNS
    )
};

