#include <Mouse.h>
#include <Keyboard.h>

#include "src/config.h"
#include "src/led_control.h"
#include "src/trackball.h"
#include "src/buttons.h"

/* ===== CONFIG DEFINITIONS ===== */
uint32_t SKY_BLUE = 0x40A0FF;
const int LED_INTERVAL_MS = 200;

const double SENSITIVITY_X = 0.05;
const double SENSITIVITY_Y = 0.10;
const double ACCEL_POWER  = 1.35;
const double GLOBAL_SENSITIVITY = 1.5;

const double SENSITIVITY_SCROLL = 0.15;
const bool invertScroll = true;

bool invertX = false;
bool invertY = false;
bool swapXY = true;

/* ===== STATE ===== */
bool scrollMode = false;

void setup() {
  Mouse.begin();
  initButtons();
  initTrackball();
  initLed();
}

void loop() {
  handleButtons(scrollMode);
  handleTrackball(scrollMode);
  updateLedAnimation();
}
