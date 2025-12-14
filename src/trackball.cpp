#include "trackball.h"
#include "config.h"
#include <SPI.h>
#include <Mouse.h>
#include <cmath>

/* ===== PAW3805 SPI ===== */
constexpr int CS_PIN = 17;
constexpr uint32_t SPI_CLOCK_SPEED = 2e6;  // 2 MHz
constexpr BitOrder SPI_DATA_ORDER = MSBFIRST;
constexpr uint8_t SPI_MODE_SETTING = SPI_MODE3;

SPISettings pawSPI(SPI_CLOCK_SPEED, SPI_DATA_ORDER, SPI_MODE_SETTING);

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

/* ===== TRACKBALL STATE ===== */
static double runX = 0, runY = 0;
const int POLLRATE = 100;
static uint32_t lastPoll = 0;

/* ===== SPI helpers ===== */
static uint8_t readRegister(PAWReg reg) {
  SPI.beginTransaction(pawSPI);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(reg);
  uint8_t val = SPI.transfer(0x80);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
  return val;
}

static void writeRegister(PAWReg reg, uint8_t val) {
  SPI.beginTransaction(pawSPI);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(reg);
  SPI.transfer(val);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
}

static int16_t readDelta(PAWReg regHi, PAWReg regLo) {
  int16_t hi = readRegister(regHi);
  int16_t lo = readRegister(regLo);
  return -1 * ((hi << 8) | lo);
}

static bool sensorReady() {
  return readRegister(MOTION_STATUS) != 0;
}

/* ===== INIT ===== */
void initTrackball() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  SPI.begin();

  // initialization sequence
  readRegister(MOTION_STATUS);
  writeRegister(OP_MODE, 0xB8);
  writeRegister(SLEEP1, 0x77);
  writeRegister(ORIENTATION, 0x14);
  writeRegister(MFIO_CONFIG, 0x34);
  writeRegister(CPI_X, 0x27);
  writeRegister(CPI_Y, 0x2B);
  delay(1000);
}

/* ===== HANDLE TRACKBALL ===== */
void handleTrackball(bool scrollMode) {
  if (micros() - lastPoll < 1000000 / POLLRATE) return;
  lastPoll = micros();

  if (!sensorReady()) return;

  int16_t dx = readDelta(DELTA_X_HI, DELTA_X_LO);
  int16_t dy = readDelta(DELTA_Y_HI, DELTA_Y_LO);

  double x = pow(fabs(dx), ACCEL_POWER) * (dx < 0 ? -1 : 1) * SENSITIVITY_X;
  double y = pow(fabs(dy), ACCEL_POWER) * (dy < 0 ? -1 : 1) * SENSITIVITY_Y;

  if (invertX) x = -x;
  if (invertY) y = -y;
  if (swapXY) std::swap(x, y);

  if (scrollMode) {
    double scrollAmount = (x + y) * SENSITIVITY_SCROLL;
    if (invertScroll) scrollAmount = -scrollAmount;

    int wheel = round(scrollAmount);
    if (wheel != 0)
      Mouse.move(0, 0, wheel);

    int moveX = round(x * GLOBAL_SENSITIVITY);
    if (moveX != 0)
      Mouse.move(moveX, 0);

    return;
  }

  // normal movement
  runX += x * GLOBAL_SENSITIVITY;
  runY += y * GLOBAL_SENSITIVITY;

  int moveX = (int)runX;
  int moveY = (int)runY;

  if (moveX != 0 || moveY != 0) {
    Mouse.move(moveX, moveY);
    runX -= moveX;
    runY -= moveY;
  }
}
