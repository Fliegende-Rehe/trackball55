#pragma once
#include <stdint.h>

void initLed();
void updateLedAnimation();

// animations
void ledRotateAnimation();
void ledBreatheAnimation();

// globals from main
extern uint32_t SKY_BLUE;
extern const int LED_INTERVAL_MS;
