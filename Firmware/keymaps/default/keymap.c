#include QMK_KEYBOARD_H


#define ENCODER_BUTTON_PIN GP2


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_UP,
        KC_DOWN,
        KC_LEFT,
        KC_RIGHT
    )
};


bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }

    return false;
}


void keyboard_post_init_user(void) {
    gpio_set_pin_input_high(ENCODER_BUTTON_PIN);
}

void matrix_scan_user(void) {
    static bool last_state = true;
    static uint32_t last_change = 0;

    bool current_state = gpio_read_pin(ENCODER_BUTTON_PIN);

    if (current_state != last_state &&
        timer_elapsed32(last_change) > 30) {

        last_state = current_state;
        last_change = timer_read32();

        // Active-low button: LOW = pressed
        if (!current_state) {
            tap_code(KC_MUTE);
        }
    }
}


#ifdef OLED_ENABLE

bool oled_task_user(void) {
    oled_write_ln_P(PSTR("BUGPAD"), false);
    oled_write_ln_P(PSTR("-----------"), false);
    oled_write_ln_P(PSTR("UP DOWN"), false);
    oled_write_ln_P(PSTR("LEFT RIGHT"), false);
    oled_write_ln_P(PSTR("ENC: VOL"), false);
    oled_write_ln_P(PSTR("PRESS: MUTE"), false);

    return false;
}

#endif
