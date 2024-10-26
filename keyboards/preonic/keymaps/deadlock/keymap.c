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
  K1,
  K2,
  K3,
  K4
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    tap_dance_action_t *action;

    switch (keycode) {
        case TD(K1):
        case TD(K2):
        case TD(K3):
        case TD(K4):
            action = &tap_dance_actions[TD_INDEX(keycode)];
            if (!record->event.pressed && action->state.count && !action->state.finished) {
                tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
                tap_code16(tap_hold->tap);
            }
            break;
    }
    return true;
}

tap_dance_action_t tap_dance_actions[] = {
  [K1] = ACTION_TAP_DANCE_TAP_HOLD(KC_1, LALT(KC_1)),
  [K2] = ACTION_TAP_DANCE_TAP_HOLD(KC_2, LALT(KC_2)),
  [K3] = ACTION_TAP_DANCE_TAP_HOLD(KC_3, LALT(KC_3)),
  [K4] = ACTION_TAP_DANCE_TAP_HOLD(KC_4, LALT(KC_4)),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_preonic_grid(
            KC_F1,   KC_F6,       KC_NO,KC_NO,         KC_GRV,   KC_ESC,    TD(K1),    TD(K2),     TD(K3),    TD(K4),    KC_5,     KC_NO,
            KC_F2,   KC_F7,       KC_NO,KC_NO,         KC_U,     KC_Y,      KC_Q,      KC_W,       KC_E,      KC_R,      KC_T,     KC_NO,
            KC_F3,   KC_F8,       KC_NO,KC_NO,         KC_J,     KC_H,      KC_TAB,    KC_S,       KC_D,      KC_F,      KC_G,     KC_NO,
            KC_F4,   KC_F9,       KC_NO,KC_NO,         KC_ESC,   KC_LSFT,   KC_Z,      KC_X,       KC_C,      KC_V,      KC_NO,    KC_NO,
            KC_F5,   KC_F10,      KC_NO,KC_NO,         KC_LALT,  KC_LCTL,   KC_P,      KC_B,       KC_SLSH,   KC_LCTL,   KC_SPC,   OSL(1)
    ),
    [1] = LAYOUT_preonic_grid(
            QK_BOOT, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_TRNS,   KC_F1,     KC_F2,      KC_F3,     KC_F4,    KC_F5,     KC_TRNS,
            KC_TRNS, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,
            KC_TRNS, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,    KC_TRNS,   KC_TRNS,  KC_TRNS,   KC_TRNS,
            KC_TRNS, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_TRNS,   SP_LAST,   MUTE_MIC,   PLAY,      KC_TRNS,  KC_TRNS,   KC_TRNS,
            KC_TRNS, KC_TRNS,     KC_TRNS,KC_TRNS,     KC_TRNS,  KC_TRNS,   SP_REC,    MUTE_APP,   NEXT,      KC_TRNS,  KC_TRNS,   KC_TRNS
    )
};
