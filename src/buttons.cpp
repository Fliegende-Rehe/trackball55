#include "buttons.h"
#include "config.h"
#include <Mouse.h>
#include <Arduino.h>

constexpr int BTN_LEFT  = 14;
constexpr int BTN_RIGHT = 15;

static uint32_t lastIdle = 0;
static uint32_t pressTime = 0;
static bool rightHeld = false;

void initButtons() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
}

/* ZMK-like hold-tap */
void handleButtons(bool &scrollMode) {
  bool left  = digitalRead(BTN_LEFT)  == LOW;
  bool right = digitalRead(BTN_RIGHT) == LOW;
  uint32_t now = millis();

  /* LEFT = pure mouse left */
  if (left) Mouse.press(MOUSE_LEFT);
  else      Mouse.release(MOUSE_LEFT);

  /* RIGHT = hold → scroll, tap → right click */
  if (right && !rightHeld) {
    if (now - lastIdle >= REQUIRE_PRIOR_IDLE_MS)
      pressTime = now;
    rightHeld = true;
  }

  if (rightHeld) {
    if (right && (now - pressTime >= TAPPING_TERM_MS)) {
      scrollMode = true;
    }

    if (!right) {
      if (now - pressTime <= QUICK_TAP_MS) {
        Mouse.click(MOUSE_RIGHT);
      }
      scrollMode = false;
      rightHeld = false;
      lastIdle = now;
    }
  }
}
