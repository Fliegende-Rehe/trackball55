#include "src/config.h"
#include "src/led_control.h"
#include "src/trackball.h"
#include "src/buttons.h"

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
