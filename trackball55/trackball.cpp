#include "trackball.h"
#include <SPI.h>
#include <Mouse.h>
#include <cmath>
#include <limits>

// --- SPI / sensor config ---
constexpr int CS_PIN = 17;
constexpr uint32_t SPI_CLOCK_SPEED = 2000000;
SPISettings pawSPI(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE3);

// --- PAW registers ---
enum PAWReg : uint8_t {
  MOTION_STATUS = 0x02,
  DELTA_X_LO = 0x03,
  DELTA_Y_LO = 0x04,
  DELTA_X_HI = 0x11,
  DELTA_Y_HI = 0x12,
  OP_MODE = 0x05,
  SLEEP1 = 0x0A
};

static double running_pos_x = 0;
static double running_pos_y = 0;

static double rollSum = 0;
static double pitchSum = 0;

constexpr int POLLRATE = 100;
constexpr double MICROS_PER_POLL = 1000000.0 / POLLRATE;
static uint32_t lastPollMicros = 0;

// --- SPI helpers ---
static uint8_t readRegister(PAWReg reg) {
  SPI.beginTransaction(pawSPI);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(reg);
  uint8_t val = SPI.transfer(0x80);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
  return val;
}

static int16_t readDelta(PAWReg hi, PAWReg lo) {
  return -((readRegister(hi) << 8) | readRegister(lo));
}

void initTrackball() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  SPI.begin();

  readRegister(OP_MODE);
  readRegister(SLEEP1);
}

void handleTrackball() {
  if (readRegister(MOTION_STATUS)) {
    rollSum += readDelta(DELTA_X_HI, DELTA_X_LO);
    pitchSum += readDelta(DELTA_Y_HI, DELTA_Y_LO);
  }

  if (micros() - lastPollMicros < MICROS_PER_POLL) return;
  lastPollMicros = micros();

  double mx = pow(fabs(rollSum), ACCEL_POWER) * (rollSum < 0 ? -1 : 1) * SENSITIVITY_X;
  double my = pow(fabs(pitchSum), ACCEL_POWER) * (pitchSum < 0 ? -1 : 1) * SENSITIVITY_Y;

  mx *= GLOBAL_SENSITIVITY;
  my *= GLOBAL_SENSITIVITY;

  if (invertX) mx = -mx;
  if (invertY) my = -my;
  if (swapXY) std::swap(mx, my);

  int out_x = round(mx);
  int out_y = round(my);

  Mouse.move(out_x, out_y);

  rollSum = 0;
  pitchSum = 0;
}
