#include "src/config.h"
#include "src/leds.h"
#include "src/trackball.h"
#include "src/buttons.h"

uint8_t curState = MOUSE_STATE;

void setup() {
  Mouse.begin();
  initButtons();
  initTrackball();
  initLed();
}

void loop() {
  handleButtons(curState);
  handleTrackball(curState);
  updateLedAnimation(curState);
}
