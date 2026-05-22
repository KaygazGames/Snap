#pragma once
#include <esp_camera.h>
#include "SettingsManager.h"

class CameraManager {
public:
  bool begin(SettingsManager& settings);
  camera_fb_t* captureWarmupAndGetFrame();
  void release(camera_fb_t* fb);
  uint8_t estimateLuma(const uint8_t* data, size_t len) const;
private:
  SettingsManager* settings {nullptr};
};
