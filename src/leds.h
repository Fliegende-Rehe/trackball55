#include <cstdint>
#pragma once

void initLed();
void updateLedAnimation(uint8_t &curState);

void ledRotateAnimation(uint8_t &curState, uint32_t ledIntervalMS);
void ledBreatheAnimation(uint8_t &curState, uint32_t ledIntervalMS);
void ledWaveAnimation(uint8_t &curState, uint32_t ledIntervalMS);
void ledSolidAnimation(uint8_t &curState);

void ledRainbowAnimation(uint32_t ledIntervalMS);
