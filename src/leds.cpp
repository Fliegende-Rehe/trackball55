#include "leds.h"
#include "config.h"

static Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

static uint32_t lastLedUpdate = 0;
static int ledIndex = 0;

static int breathe = 0;
static bool breatheUp = true;

void initLed() {
  leds.begin();
  leds.setBrightness(LED_BRIGHTNESS);
  leds.show();
}

void updateLedAnimation(bool scrollMode) {
  // ledRotateAnimation(scrollMode);
  ledBreatheAnimation(scrollMode);
}

/* ---------- ROTATE ---------- */
void ledRotateAnimation(bool scrollMode) {
    static uint32_t lastUpdate = 0;
    static int index = 0;

    uint32_t now = millis();
    if (now - lastUpdate < LED_INTERVAL_MS)
        return;

    lastUpdate = now;

    uint32_t color = scrollMode ? LED_SCROLL_COLOR : LED_MOUSE_COLOR;

    for (int i = 0; i < LED_COUNT; i++)
        leds.setPixelColor(i, 0);

    leds.setPixelColor(index, color);

    index++;
    if (index >= LED_COUNT)
        index = 0;

    leds.show();
}

/* ---------- BREATHE ---------- */
void ledBreatheAnimation(bool scrollMode) {
    static uint32_t lastUpdate = 0;
    static float phase = 0.0f;  // progress through sine wave

    uint32_t now = millis();
    if (now - lastUpdate < 20)  // update ~50Hz
        return;

    lastUpdate = now;

    // Calculate brightness using sine wave
    float brightnessFactor = (sinf(phase) + 1.0f) / 2.0f; // 0..1
    uint32_t baseColor = scrollMode ? LED_SCROLL_COLOR : LED_MOUSE_COLOR;

    uint8_t r = ((baseColor >> 16) & 0xFF) * brightnessFactor;
    uint8_t g = ((baseColor >> 8) & 0xFF) * brightnessFactor;
    uint8_t b = (baseColor & 0xFF) * brightnessFactor;

    uint32_t color = leds.Color(r, g, b);

    // Set all LEDs to same color
    for (int i = 0; i < LED_COUNT; i++)
        leds.setPixelColor(i, color);

    leds.show();

    // Advance phase
    phase += 0.05f; // adjust speed here
    if (phase >= 2.0f * 3.14159265f)
        phase -= 2.0f * 3.14159265f;
}
