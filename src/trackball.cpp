#include "trackball.h"
#include "config.h"

/* ================= SPI ================= */

static SPISettings pawSPI(
    SPI_CLOCK_SPEED,
    SPI_DATA_ORDER,
    SPI_MODE_SETTING
);

/* ================= State ================= */

static double accX = 0.0;
static double accY = 0.0;

static double fracX = 0.0;
static double fracY = 0.0;

static uint32_t lastPollMicros = 0;
static constexpr int POLLRATE = 100;
static constexpr uint32_t MICROS_PER_POLL = 1000000UL / POLLRATE;

/* ================= Low-level SPI ================= */

static uint8_t readRegister(PAWReg reg) {
    SPI.beginTransaction(pawSPI);
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(reg);
    uint8_t v = SPI.transfer(0x80);
    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
    return v;
}

static void writeRegister(PAWReg reg, uint8_t val) {
    SPI.beginTransaction(pawSPI);
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(reg);
    SPI.transfer(val);
    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
}

/* ================= Sensor ================= */

static bool sensorReady() {
    return readRegister(MOTION_STATUS) != 0;
}

static int16_t readDelta(PAWReg hi, PAWReg lo) {
    int16_t h = readRegister(hi);
    int16_t l = readRegister(lo);
    return (int16_t)((h << 8) | l);
}

/* ================= Init ================= */

void initTrackball() {
    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);
    SPI.begin();

    writeRegister(OP_MODE,     0xB8);
    writeRegister(SLEEP1,      0x77);
    writeRegister(ORIENTATION, 0x14);
    writeRegister(MFIO_CONFIG, 0x34);
    writeRegister(CPI_X,       0x27);
    writeRegister(CPI_Y,       0x2B);

    delay(100);
}

/* ================= Main handler ================= */

void handleTrackball(uint8_t &curState) {
    if (!sensorReady())
        return;

    /* accumulate raw motion */
    accX += readDelta(DELTA_X_HI, DELTA_X_LO);
    accY += readDelta(DELTA_Y_HI, DELTA_Y_LO);

    uint32_t now = micros();
    if (now - lastPollMicros < MICROS_PER_POLL)
        return;

    lastPollMicros = now;

    double x = accX;
    double y = accY;
    accX = 0;
    accY = 0;

    /* axis config */
    if (invertX) x = -x;
    if (invertY) y = -y;
    if (swapXY)  std::swap(x, y);

    /* ================= SCROLL MODE ================= */
    if (curState == SCROLL_STATE) {
        x *= SENSITIVITY_SCROLL;
        y *= SENSITIVITY_SCROLL;

        if (invertScroll) {
            x = -x;
            y = -y;
        }

        fracX += x;
        fracY += y;

        int sx = (int)round(fracX);
        int sy = (int)round(fracY);

        fracX -= sx;
        fracY -= sy;

        constexpr int8_t MAX = 127;

        while (sx || sy) {
            int8_t dx = constrain(sx, -MAX, MAX);
            int8_t dy = constrain(sy, -MAX, MAX);

            Mouse.move(dx, 0, dy);   // horizontal + vertical scroll

            sx -= dx;
            sy -= dy;
        }

        return;
    }

    /* ================= MOUSE MODE ================= */

    x *= GLOBAL_SENSITIVITY;
    y *= GLOBAL_SENSITIVITY;

    /* acceleration */
    x = copysign(pow(fabs(x), ACCEL_POWER), x);
    y = copysign(pow(fabs(y), ACCEL_POWER), y);

    fracX += x;
    fracY += y;

    int mx = (int)round(fracX);
    int my = (int)round(fracY);

    fracX -= mx;
    fracY -= my;

    constexpr int8_t MAX = 127;

    while (mx || my) {
        int8_t dx = constrain(mx, -MAX, MAX);
        int8_t dy = constrain(my, -MAX, MAX);

        Mouse.move(dx, dy);

        mx -= dx;
        my -= dy;
    }
}
