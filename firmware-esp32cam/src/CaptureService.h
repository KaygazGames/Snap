#pragma once
#include <Arduino.h>
#include "SettingsManager.h"
#include "SdStorage.h"
#include "MetadataStore.h"
#include "CameraManager.h"
#include "FlashController.h"

class CaptureService {
public:
  void begin(SettingsManager& settings, SdStorage& storage, MetadataStore& metadata, CameraManager& camera, FlashController& flash, uint8_t buttonPin);
  void loop();
  bool triggerCapture();
private:
  SettingsManager* settings {nullptr};
  SdStorage* storage {nullptr};
  MetadataStore* metadata {nullptr};
  CameraManager* camera {nullptr};
  FlashController* flash {nullptr};
  uint8_t pin {0};
  bool prevState {HIGH};
  uint32_t lastDebounceMs {0};
  bool shouldCapture();
  bool capture();
};
