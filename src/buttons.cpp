#include "buttons.h"
#include "config.h"

/* ================= CONFIG ================= */

constexpr uint16_t DEBOUNCE_MS = 20;

/* ================= STATE ================= */

struct Button {
    bool phys = false;       // raw
    bool stable = false;     // debounced
    uint32_t lastChange = 0;

    bool pressed = false;
    uint32_t pressTime = 0;
};

static Button leftBtn;
static Button rightBtn;

static bool middleActive = false;
static bool rightHoldDecided = false;

/* ================= DEBOUNCE ================= */

static bool debounce(Button &b, bool raw, uint32_t now) {
    if (raw != b.phys) {
        b.phys = raw;
        b.lastChange = now;
    }

    if ((now - b.lastChange) >= DEBOUNCE_MS) {
        if (b.stable != raw) {
            b.stable = raw;
            return true; // edge
        }
    }
    return false;
}

/* ================= INIT ================= */

void initButtons() {
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
}

/* ================= HANDLER ================= */

void handleButtons(bool &scrollMode) {
    uint32_t now = millis();

    bool rawLeft  = digitalRead(BTN_LEFT)  == LOW;
    bool rawRight = digitalRead(BTN_RIGHT) == LOW;

    bool leftEdge  = debounce(leftBtn, rawLeft, now);
    bool rightEdge = debounce(rightBtn, rawRight, now);

    /* ================= COMBO: MIDDLE CLICK ================= */

    if (!middleActive &&
        leftBtn.stable &&
        rightBtn.stable) {

        middleActive = true;
        Mouse.press(MOUSE_MIDDLE);

        // cancel other behaviors
        scrollMode = false;
        rightHoldDecided = true;

        Mouse.release(MOUSE_LEFT);
    }

    if (middleActive &&
        (!leftBtn.stable || !rightBtn.stable)) {

        Mouse.release(MOUSE_MIDDLE);
        middleActive = false;
    }

    /* ================= LEFT BUTTON ================= */

    if (!middleActive && leftEdge) {
        if (leftBtn.stable) {
            Mouse.press(MOUSE_LEFT);
        } else {
            Mouse.release(MOUSE_LEFT);
        }
    }

    /* ================= RIGHT BUTTON ================= */

    if (rightEdge) {
        if (rightBtn.stable) {
            rightBtn.pressed = true;
            rightBtn.pressTime = now;
            rightHoldDecided = false;
        } else {
            // released
            if (!rightHoldDecided && !middleActive) {
                uint32_t dt = now - rightBtn.pressTime;
                if (dt <= QUICK_TAP_MS) {
                    Mouse.click(MOUSE_RIGHT);
                }
            }

            scrollMode = false;
            rightBtn.pressed = false;
            rightHoldDecided = false;
        }
    }

    /* ================= RIGHT HOLD → SCROLL ================= */

    if (!middleActive &&
        rightBtn.pressed &&
        !rightHoldDecided &&
        (now - rightBtn.pressTime >= TAPPING_TERM_MS)) {

        scrollMode = true;
        rightHoldDecided = true;
    }
}
