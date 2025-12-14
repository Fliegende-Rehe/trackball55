#include "led_control.h"
#include <Adafruit_NeoPixel.h>

constexpr int LED_PIN = 13;
constexpr int LED_COUNT = 11;

static Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// shared timing
static uint32_t lastLedUpdate = 0;

// ===== ROTATE STATE =====
static int ledIndex = 0;

// ===== BREATHE STATE =====
static int breatheValue = 0;
static int breatheStep = 3;   // speed of breathing
static bool breatheUp = true;

// ======================================================

void initLed() {
  leds.begin();
  leds.setBrightness(50);
  leds.show();
}

// ======================================================
// Dispatcher (called from loop)
// ======================================================
void updateLedAnimation() {
  // choose animation here
  // ledRotateAnimation();
  ledBreatheAnimation();
}

// ======================================================
// Animation 1: rotating pixel (OLD animation)
// ======================================================
void ledRotateAnimation() {
  uint32_t now = millis();
  if (now - lastLedUpdate < LED_INTERVAL_MS) return;
  lastLedUpdate = now;

  leds.clear();
  leds.setPixelColor(ledIndex, SKY_BLUE);

  ledIndex = (ledIndex + 1) % LED_COUNT;
  leds.show();
}

// ======================================================
// Animation 2: breathe (NEW)
// ======================================================
void ledBreatheAnimation() {
  uint32_t now = millis();
  if (now - lastLedUpdate < LED_INTERVAL_MS) return;
  lastLedUpdate = now;

  // update breathe value
  if (breatheUp) {
    breatheValue += breatheStep;
    if (breatheValue >= 255) {
      breatheValue = 255;
      breatheUp = false;
    }
  } else {
    breatheValue -= breatheStep;
    if (breatheValue <= 0) {
      breatheValue = 0;
      breatheUp = true;
    }
  }

  uint8_t r = (SKY_BLUE >> 16) & 0xFF;
  uint8_t g = (SKY_BLUE >> 8) & 0xFF;
  uint8_t b = SKY_BLUE & 0xFF;

  // scale color by breatheValue
  r = (r * breatheValue) / 255;
  g = (g * breatheValue) / 255;
  b = (b * breatheValue) / 255;

  for (int i = 0; i < LED_COUNT; i++)
    leds.setPixelColor(i, r, g, b);

  leds.show();
}
