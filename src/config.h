#pragma once
#include <stdint.h>

/* ================= TIMING ================= */
constexpr uint16_t QUICK_TAP_MS          = 150;
constexpr uint16_t TAPPING_TERM_MS       = 200;
constexpr uint16_t REQUIRE_PRIOR_IDLE_MS = 100;

/* ================= LED ================= */
extern uint32_t SKY_BLUE;
extern const int LED_INTERVAL_MS;

/* ================= TRACKBALL ================= */
extern const double SENSITIVITY_X;
extern const double SENSITIVITY_Y;
extern const double ACCEL_POWER;
extern const double GLOBAL_SENSITIVITY;

/* ================= SCROLL ================= */
extern const double SENSITIVITY_SCROLL;
extern const bool invertScroll;

/* ================= AXIS ================= */
extern bool invertX;
extern bool invertY;
extern bool swapXY;
