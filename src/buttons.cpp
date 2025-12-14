#include "config.h"
#include "buttons.h"

struct ButtonState {
    bool pressed      = false;  // current physical state
    bool held         = false;  // hold detected
    bool tapSent      = false;  // tap already sent
    uint32_t pressTime = 0;     // time of press
};

static ButtonState leftBtn;
static ButtonState rightBtn;
static uint32_t lastIdle = 0;

void initButtons() {
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
}

/* ZMK-like hold-tap for RIGHT, LEFT = normal */
void handleButtons(bool &scrollMode) {
    uint32_t now = millis();

    // ---------------- LEFT BUTTON ----------------
    bool leftPressed = digitalRead(BTN_LEFT) == LOW;
    if (leftPressed && !leftBtn.pressed) {
        leftBtn.pressed = true;
        leftBtn.pressTime = now;
        Mouse.press(MOUSE_LEFT);
    } else if (!leftPressed && leftBtn.pressed) {
        leftBtn.pressed = false;
        Mouse.release(MOUSE_LEFT);
        lastIdle = now;
    }

    // ---------------- RIGHT BUTTON ----------------
    bool rightPressed = digitalRead(BTN_RIGHT) == LOW;

    if (rightPressed && !rightBtn.pressed) {
        // Button just pressed
        rightBtn.pressed = true;
        rightBtn.held = false;
        rightBtn.tapSent = false;
        rightBtn.pressTime = now;
    }

    if (rightBtn.pressed) {
        uint32_t heldTime = now - rightBtn.pressTime;

        // If held long enough → scroll mode
        if (!rightBtn.held && heldTime >= TAPPING_TERM_MS) {
            scrollMode = true;
            rightBtn.held = true;
        }

        // On release
        if (!rightPressed) {
            if (!rightBtn.held && !rightBtn.tapSent && heldTime <= QUICK_TAP_MS) {
                // Quick tap → send right click once
                Mouse.click(MOUSE_RIGHT);
                rightBtn.tapSent = true;
            }
            scrollMode = false;
            rightBtn.pressed = false;
            rightBtn.held = false;
            rightBtn.tapSent = false;
            lastIdle = now;
        }
    }

    // ---------------- Idle tracking ----------------
    if (!leftBtn.pressed && !rightBtn.pressed) lastIdle = now;
}
