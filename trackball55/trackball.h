#pragma once

void initTrackball();
void handleTrackball();

// === extern globals ===
extern const double SENSITIVITY_X;
extern const double SENSITIVITY_Y;
extern const double ACCEL_POWER;
extern const double GLOBAL_SENSITIVITY;
extern const double SCROLL_SENSITIVITY;

extern bool invertX;
extern bool invertY;
extern bool swapXY;
extern bool INVERT_SCROLL;
