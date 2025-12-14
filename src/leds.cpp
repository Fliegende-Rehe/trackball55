#include "leds.h"
#include "config.h"

static Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

static uint32_t lastLedUpdate = 0;
static int ledIndex = 0;

static int breathe = 0;
static bool breatheUp = true;

void initLed() {
  leds.begin();
  leds.setBrightness(50);
  leds.show();
}

void updateLedAnimation() {
  ledRotateAnimation();
  // ledBreatheAnimation();
}

/* ---------- ROTATE ---------- */
void ledRotateAnimation() {
  if (millis() - lastLedUpdate < LED_INTERVAL_MS) return;
  lastLedUpdate = millis();

  leds.clear();
  leds.setPixelColor(ledIndex, LED_COLOR);
  ledIndex = (ledIndex + 1) % LED_COUNT;
  leds.show();
}

/* ---------- BREATHE ---------- */
void ledBreatheAnimation() {
  if (millis() - lastLedUpdate < LED_INTERVAL_MS) return;
  lastLedUpdate = millis();

  breathe += breatheUp ? 4 : -4;
  if (breathe >= 255) breatheUp = false;
  if (breathe <= 0)   breatheUp = true;

  uint8_t r = ((LED_COLOR >> 16) & 0xFF) * breathe / 255;
  uint8_t g = ((LED_COLOR >> 8)  & 0xFF) * breathe / 255;
  uint8_t b = (LED_COLOR & 0xFF) * breathe / 255;

  for (int i = 0; i < LED_COUNT; i++)
    leds.setPixelColor(i, r, g, b);

  leds.show();
}
