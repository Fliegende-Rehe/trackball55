#include "config.h"

/* ================= LED ================= */
uint32_t LED_COLOR = 0x40A0FF;
const int LED_INTERVAL_MS = 100;

/* ================= TRACKBALL ================= */
const double SENSITIVITY_X = 0.05;
const double SENSITIVITY_Y = 0.09;
const double ACCEL_POWER  = 1.5;
const double GLOBAL_SENSITIVITY = 0.1;

/* ================= SCROLL ================= */
const double SENSITIVITY_SCROLL = 0.005;
const bool invertScroll = true;

/* ================= AXIS ================= */
bool invertX = true;
bool invertY = true;
bool swapXY = true;
