#pragma once
#include <Arduino.h>
#include "SettingsManager.h"

class FlashController {
public:
  void begin(uint8_t pin, SettingsManager& settings);
  void applyMode();
  bool shouldAutoFire(uint8_t luma) const;
  void pulseForCapture();
private:
  uint8_t ledPin {4};
  SettingsManager* settings {nullptr};
};
