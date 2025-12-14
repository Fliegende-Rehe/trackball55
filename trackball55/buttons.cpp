#include "buttons.h"
#include <Mouse.h>

constexpr int BTN_LEFT = 14;
constexpr int BTN_RIGHT = 15;

void initButtons() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
}

void handleButtons() {
  static bool lPrev = false;
  static bool rPrev = false;

  bool lNow = digitalRead(BTN_LEFT) == LOW;
  bool rNow = digitalRead(BTN_RIGHT) == LOW;

  if (lNow != lPrev) lNow ? Mouse.press(MOUSE_LEFT) : Mouse.release(MOUSE_LEFT);
  if (rNow != rPrev) rNow ? Mouse.press(MOUSE_RIGHT) : Mouse.release(MOUSE_RIGHT);

  lPrev = lNow;
  rPrev = rNow;
}
