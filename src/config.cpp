#include "config.h"

/* ================= LED ================= */
uint32_t LED_COLOR = 0x40A0FF;
const int LED_INTERVAL_MS = 1001;

/* ================= TRACKBALL ================= */
const double SENSITIVITY_X = 0.05;
const double SENSITIVITY_Y = 0.09;
const double ACCEL_POWER  = 1.35;
const double GLOBAL_SENSITIVITY = 1.1;

/* ================= SCROLL ================= */
const double SENSITIVITY_SCROLL = 0.05;
const bool invertScroll = true;

/* ================= AXIS ================= */
bool invertX = false;
bool invertY = false;
bool swapXY = true;
