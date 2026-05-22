#include "FlashController.h"

void FlashController::begin(uint8_t pin, SettingsManager& s) {
  ledPin = pin; settings = &s;
  ledcAttach(ledPin, 5000, 8);
  applyMode();
}
void FlashController::applyMode() {
  auto mode = settings->get().flashMode;
  if (mode == FLASH_OFF || mode == FLASH_AUTO) ledcWrite(ledPin, 0);
  else ledcWrite(ledPin, settings->get().flashIntensity);
}
bool FlashController::shouldAutoFire(uint8_t luma) const {
  return settings->get().flashMode == FLASH_AUTO && luma < settings->get().autoFlashThreshold;
}
void FlashController::pulseForCapture() {
  ledcWrite(ledPin, settings->get().flashIntensity);
  delay(120);
  if (settings->get().flashMode != FLASH_TORCH && settings->get().flashMode != FLASH_ON) ledcWrite(ledPin, 0);
}
