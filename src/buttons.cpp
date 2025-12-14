#include "buttons.h"
#include "config.h"

constexpr uint16_t DEBOUNCE_MS = 20;

struct Button {
    bool physState = false;      // raw read
    bool stableState = false;    // debounced
    uint32_t lastChange = 0;

    bool active = false;         // logical pressed
    bool decided = false;        // tap/hold decided
    uint32_t pressTime = 0;
};

static Button leftBtn;
static Button rightBtn;

static bool debounce(Button &b, bool raw, uint32_t now) {
    if (raw != b.physState) {
        b.physState = raw;
        b.lastChange = now;
    }

    if ((now - b.lastChange) >= DEBOUNCE_MS) {
        if (b.stableState != raw) {
            b.stableState = raw;
            return true; // edge
        }
    }
    return false;
}

void initButtons() {
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
}

void handleButtons(bool &scrollMode) {
    uint32_t now = millis();

    bool rawLeft  = digitalRead(BTN_LEFT)  == LOW;
    bool rawRight = digitalRead(BTN_RIGHT) == LOW;

    // ================= LEFT BUTTON =================
    if (debounce(leftBtn, rawLeft, now)) {
        if (leftBtn.stableState) {
            // pressed
            Mouse.press(MOUSE_LEFT);
        } else {
            // released
            Mouse.release(MOUSE_LEFT);
        }
    }

    // ================= RIGHT BUTTON =================
    if (debounce(rightBtn, rawRight, now)) {

        if (rightBtn.stableState) {
            // pressed
            rightBtn.active = true;
            rightBtn.decided = false;
            rightBtn.pressTime = now;
        } else {
            // released
            if (rightBtn.active && !rightBtn.decided) {
                uint32_t dt = now - rightBtn.pressTime;
                if (dt <= QUICK_TAP_MS) {
                    Mouse.click(MOUSE_RIGHT);
                }
            }

            scrollMode = false;
            rightBtn.active = false;
            rightBtn.decided = false;
        }
    }

    // HOLD detection (scroll mode)
    if (rightBtn.active && !rightBtn.decided) {
        if ((now - rightBtn.pressTime) >= TAPPING_TERM_MS) {
            scrollMode = true;
            rightBtn.decided = true; // lock decision
        }
    }
}
