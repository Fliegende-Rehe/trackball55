#pragma once

#include <Mouse.h>
// #include <Keyboard.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <cmath>

/* ================= TIMING ================= */
constexpr uint16_t QUICK_TAP_MS          = 180;
constexpr uint16_t TAPPING_TERM_MS       = 200;
constexpr uint16_t REQUIRE_PRIOR_IDLE_MS = 150;

/* ---------- DEVICE STATE ---------- */
enum DeviceState : uint8_t {
    MOUSE_STATE  = 0,
    SCROLL_STATE = 1,
    MIDDLE_STATE = 2
};

/* ================= LED ================= */
extern uint32_t LED_MOUSE_COLOR;
extern uint32_t LED_SCROLL_COLOR;
extern uint32_t LED_MIDDLE_COLOR;
extern uint8_t LED_BRIGHTNESS;

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

/* ---------- LED PARAMS ---------- */
constexpr int LED_PIN   = 13;
constexpr int LED_COUNT = 10;

/* ---------- BUTTON PINS ---------- */
constexpr uint8_t BTN_LEFT  = 14;
constexpr uint8_t BTN_RIGHT = 15;
constexpr uint16_t DEBOUNCE_MS = 20;

/* ===== PAW3805 SPI ===== */
constexpr int CS_PIN = 17;
constexpr uint32_t SPI_CLOCK_SPEED = 2e6;  // 2 MHz
constexpr BitOrder SPI_DATA_ORDER = MSBFIRST;
constexpr uint8_t SPI_MODE_SETTING = SPI_MODE3;

/* ===== PAW3805 Registers ===== */
enum PAWReg : uint8_t {
  MOTION_STATUS = 0x02,
  DELTA_X_LO    = 0x03,
  DELTA_Y_LO    = 0x04,
  DELTA_X_HI    = 0x11,
  DELTA_Y_HI    = 0x12,
  OP_MODE       = 0x05,
  SLEEP1        = 0x0A,
  ORIENTATION   = 0x19,
  MFIO_CONFIG   = 0x26,
  CPI_X         = 0x0D,
  CPI_Y         = 0x0E
};
