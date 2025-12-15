#include "config.h"

/* ================= LED ================= */
uint32_t LED_MOUSE_COLOR = 0x40A0FF;
uint32_t LED_SCROLL_COLOR = 0x00A86B;
uint32_t LED_MIDDLE_COLOR = 0xFF8DA1;
uint8_t  LED_BRIGHTNESS  = 75;  

/* ================= TRACKBALL ================= */
const double SENSITIVITY_X = 0.05;
const double SENSITIVITY_Y = 0.09;
const double ACCEL_POWER  = 1.5;
const double GLOBAL_SENSITIVITY = 0.12;

/* ================= SCROLL ================= */
const double SENSITIVITY_SCROLL = 0.005;
const bool invertScroll = true;

/* ================= AXIS ================= */
bool invertX = true;
bool invertY = true;
bool swapXY = true;
