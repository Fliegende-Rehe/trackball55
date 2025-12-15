#include "buttons.h"
#include "config.h"

struct Button {
    bool phys = false;
    bool stable = false;
    uint32_t lastChange = 0;

    bool active = false;
    bool decided = false;
    uint32_t pressTime = 0;
};

static Button leftBtn;
static Button rightBtn;

static bool debounce(Button &b, bool raw, uint32_t now) {
    if (raw != b.phys) {
        b.phys = raw;
        b.lastChange = now;
    }

    if ((now - b.lastChange) >= DEBOUNCE_MS) {
        if (b.stable != raw) {
            b.stable = raw;
            return true;
        }
    }
    return false;
}

void initButtons() {
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
}

void handleButtons(uint8_t &curState) {
    uint32_t now = millis();

    bool rawLeft  = digitalRead(BTN_LEFT)  == LOW;
    bool rawRight = digitalRead(BTN_RIGHT) == LOW;

    /* ---------- LEFT BUTTON ---------- */
    if (debounce(leftBtn, rawLeft, now)) {
        if (leftBtn.stable)
            Mouse.press(MOUSE_LEFT);
        else
            Mouse.release(MOUSE_LEFT);
    }

    /* ---------- BOTH BUTTONS = MIDDLE ---------- */
    if (leftBtn.stable && rightBtn.stable) {
        curState = MIDDLE_STATE;
        Mouse.press(MOUSE_MIDDLE);
        return;
    } else {
        Mouse.release(MOUSE_MIDDLE);
    }

    /* ---------- RIGHT BUTTON (tap / hold) ---------- */
    if (debounce(rightBtn, rawRight, now)) {
        if (rightBtn.stable) {
            rightBtn.active = true;
            rightBtn.decided = false;
            rightBtn.pressTime = now;
        } else {
            if (rightBtn.active && !rightBtn.decided) {
                if ((now - rightBtn.pressTime) <= QUICK_TAP_MS)
                    Mouse.click(MOUSE_RIGHT);
            }

            rightBtn.active = false;
            rightBtn.decided = false;
            curState = MOUSE_STATE;
        }
    }

    if (rightBtn.active && !rightBtn.decided) {
        if ((now - rightBtn.pressTime) >= TAPPING_TERM_MS) {
            curState = SCROLL_STATE;
            rightBtn.decided = true;
        }
    }
}
