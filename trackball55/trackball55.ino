#include <Mouse.h>
#include <Keyboard.h>

#include "led_control.h"
#include "trackball.h"
#include "buttons.h"

// ================= GLOBAL CONFIG (DEFINED HERE) =================

// --- LED animation ---
uint32_t SKY_BLUE = 0x40A0FF;
constexpr int LED_INTERVAL_MS = 200;

// --- HID mouse settings ---
const double SENSITIVITY_X = 0.05;
const double SENSITIVITY_Y = 0.1;
const double ACCEL_POWER = 1.35;
const double GLOBAL_SENSITIVITY = 1.5;
const double SCROLL_SENSITIVITY = 0.1;

// --- Axis configuration ---
bool invertX = false;
bool invertY = false;
bool swapXY = true;
bool INVERT_SCROLL = true;

// --- Serial settings ---
constexpr bool ENABLE_SERIAL = false;

// --- Hold-Tap (BTN_RIGHT) ---
constexpr uint16_t QUICK_TAP_MS = 150;
constexpr uint16_t TAPPING_TERM_MS = 200;
constexpr uint16_t REQUIRE_PRIOR_IDLE_MS = 100;


// ================================================================

void setup() {
  Mouse.begin();
  Keyboard.begin();

  initTrackball();
  initButtons();
  initLed();
}

void loop() {
  handleTrackball();
  handleButtons();
  updateLedAnimation();
}
