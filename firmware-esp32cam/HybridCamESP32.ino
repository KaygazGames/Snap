#include "src/AppConfig.h"
#include "src/SettingsManager.h"
#include "src/SdStorage.h"
#include "src/MetadataStore.h"
#include "src/FlashController.h"
#include "src/CameraManager.h"
#include "src/CaptureService.h"
#include "src/BleServerManager.h"

SettingsManager settings;
SdStorage storage;
MetadataStore metadata;
FlashController flash;
CameraManager camera;
CaptureService captureService;
BleServerManager ble;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SHUTTER_BUTTON, INPUT_PULLUP);

  settings.begin();
  flash.begin(PIN_FLASH_LED, settings);
  storage.begin();
  metadata.begin(storage);
  camera.begin(settings);
  captureService.begin(settings, storage, metadata, camera, flash, PIN_SHUTTER_BUTTON);
  ble.begin(settings, storage, metadata, flash, camera, captureService);

  Serial.println("HybridCam ESP32 started");
}

void loop() {
  captureService.loop();
  ble.loop();
}
