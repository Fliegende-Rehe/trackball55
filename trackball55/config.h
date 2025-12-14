#pragma once
#include <stdint.h>

// --- HID mouse settings ---
extern const double SENSITIVITY_X;
extern const double SENSITIVITY_Y;
extern const double ACCEL_POWER;
extern const double GLOBAL_SENSITIVITY;
extern const double SCROLL_SENSITIVITY;

// --- Axis configuration ---
extern bool invertX;
extern bool invertY;
extern bool swapXY;
extern bool INVERT_SCROLL;

// --- LED config ---
extern uint32_t SKY_BLUE;
extern const int LED_INTERVAL_MS;

// --- Serial ---
constexpr bool ENABLE_SERIAL = false;

// --- ZMK-like timing ---
constexpr uint16_t QUICK_TAP_MS          = 150;
constexpr uint16_t TAPPING_TERM_MS       = 200;
constexpr uint16_t REQUIRE_PRIOR_IDLE_MS = 100;
