#include "leds.h"
#include "config.h"

static Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

static uint32_t lastLedUpdate = 0;

// --- common for all animations ---
static float phase = 0.0f;

void initLed() {
    leds.begin();
    leds.setBrightness(LED_BRIGHTNESS);
    leds.show();
}

// --- wrapper to call chosen animation ---
void updateLedAnimation(uint8_t &curState) {
    // Pass your desired interval in ms per animation
    ledBreatheAnimation(curState, 75);
    // ledRotateAnimation(curState, 100);
    // ledSolidAnimation(curState, 50);
    // ledRainbowAnimation(curState, 50);
}

// --- get LED color based on curState ---
static uint32_t getLedColor(uint8_t curState) {
    switch (curState) {
        case SCROLL_STATE: return LED_SCROLL_COLOR;
        case MIDDLE_STATE: return LED_MIDDLE_COLOR;
        default:           return LED_MOUSE_COLOR;
    }
}

// --- breathe animation ---
void ledBreatheAnimation(uint8_t &curState, uint32_t ledIntervalMS) {
    uint32_t now = millis();
    if (now - lastLedUpdate < ledIntervalMS) return;
    lastLedUpdate = now;

    uint32_t baseColor = getLedColor(curState);

    float k = (sinf(phase) + 1.0f) * 0.5f;

    uint8_t r = ((baseColor >> 16) & 0xFF) * k;
    uint8_t g = ((baseColor >> 8) & 0xFF) * k;
    uint8_t b = (baseColor & 0xFF) * k;

    for (int i = 0; i < LED_COUNT; i++)
        leds.setPixelColor(i, leds.Color(r, g, b));

    leds.show();

    phase += 0.05f;
    if (phase >= 6.2831853f) phase -= 6.2831853f;
}

// --- rotating LED animation ---
void ledRotateAnimation(uint8_t &curState, uint32_t ledIntervalMS) {
    static int ledIndex = 0;

    uint32_t now = millis();
    if (now - lastLedUpdate < ledIntervalMS) return;
    lastLedUpdate = now;

    // clear strip
    for (int i = 0; i < LED_COUNT; i++)
        leds.setPixelColor(i, 0);

    leds.setPixelColor(ledIndex, getLedColor(curState));

    ledIndex++;
    if (ledIndex >= LED_COUNT) ledIndex = 0;

    leds.show();
}

// --- solid color animation ---
void ledSolidAnimation(uint8_t &curState, uint32_t ledIntervalMS) {
    (void)ledIntervalMS; // interval not used for static color

    uint32_t baseColor = getLedColor(curState);
    for (int i = 0; i < LED_COUNT; i++)
        leds.setPixelColor(i, baseColor);

    leds.show();
}

// --- rainbow animation ---
void ledRainbowAnimation(uint8_t &curState, uint32_t ledIntervalMS) {
    static uint16_t offset = 0;

    uint32_t now = millis();
    if (now - lastLedUpdate < ledIntervalMS) return;
    lastLedUpdate = now;

    for (int i = 0; i < LED_COUNT; i++) {
        uint16_t hue = (i * 65536L / LED_COUNT + offset) & 0xFFFF;
        leds.setPixelColor(i, leds.ColorHSV(hue));
    }

    leds.show();

    offset += 256; // speed of rainbow rotation
}

// --- wave animation ---
void ledWaveAnimation(uint8_t &curState, uint32_t ledIntervalMS) {
    uint32_t now = millis();
    if (now - lastLedUpdate < ledIntervalMS) return;
    lastLedUpdate = now;

    uint32_t baseColor = getLedColor(curState);

    for (int i = 0; i < LED_COUNT; i++) {
        float k = (sinf(phase + i * 0.5f) + 1.0f) * 0.5f;
        uint8_t r = ((baseColor >> 16) & 0xFF) * k;
        uint8_t g = ((baseColor >> 8) & 0xFF) * k;
        uint8_t b = (baseColor & 0xFF) * k;
        leds.setPixelColor(i, leds.Color(r, g, b));
    }

    leds.show();

    phase += 0.1f;
    if (phase >= 6.2831853f) phase -= 6.2831853f;
}
