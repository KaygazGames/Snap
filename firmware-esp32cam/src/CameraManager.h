#pragma once
#include <esp_camera.h>
#include "SettingsManager.h"

class CameraManager {
public:
  bool begin(SettingsManager& settings);
  camera_fb_t* capture();
  void release(camera_fb_t* fb);
};
