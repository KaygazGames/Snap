#pragma once
#include <Arduino.h>
#include "SettingsManager.h"
#include "SdStorage.h"
#include "MetadataStore.h"
#include "FlashController.h"
#include "CameraManager.h"
#include "CaptureService.h"

class BleServerManager {
public:
  void begin(SettingsManager& settings, SdStorage& storage, MetadataStore& metadata, FlashController& flash, CameraManager& camera, CaptureService& capture);
  void loop();
private:
  CaptureService* captureService {nullptr};
};
