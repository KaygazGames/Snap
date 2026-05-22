#pragma once
#include <Arduino.h>
#include "SettingsManager.h"
#include "SdStorage.h"
#include "MetadataStore.h"
#include "FlashController.h"
#include "CameraManager.h"

class BleServerManager {
public:
  void begin(SettingsManager& settings, SdStorage& storage, MetadataStore& metadata, FlashController& flash, CameraManager& camera);
  void loop();
};
