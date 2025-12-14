#include "config.h"
#include "trackball.h"

SPISettings pawSPI(SPI_CLOCK_SPEED, SPI_DATA_ORDER, SPI_MODE_SETTING);

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

    // TODO: fix horizontal movement
    if (scrollMode) {
        // Scroll amounts
        int scrollH = round(x * SENSITIVITY_SCROLL);
        int scrollV = round(y * SENSITIVITY_SCROLL);
        if (invertScroll) {
            scrollH = -scrollH;
            scrollV = -scrollV;
        }

        // Vertical scroll via wheel
        if (scrollV != 0)
            Mouse.move(0, 0, scrollV);

        // Horizontal scroll workaround: map to multiple small vertical scrolls for testing
        // Or ignore if library doesn't support horizontal wheel
        if (scrollH != 0)
            Mouse.move(scrollH, 0, 0);

        return; // Disable cursor movement
    }

    // Normal cursor movement
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
