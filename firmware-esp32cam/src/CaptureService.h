#pragma once
#include <Arduino.h>
#include "SettingsManager.h"
#include "SdStorage.h"
#include "CameraManager.h"

class CaptureService {
public:
  void begin(SettingsManager& settings, SdStorage& storage, CameraManager& camera);
  void loop();
  bool snapNow(String& outPath, uint32_t& outId);
  uint32_t latestId() const { return lastId; }
private:
  SettingsManager* settings{nullptr};
  SdStorage* storage{nullptr};
  CameraManager* camera{nullptr};
  uint32_t lastId{0};
  uint32_t lastPress{0};
};
