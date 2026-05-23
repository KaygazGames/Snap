#include "src/AppConfig.h"
#include "src/SettingsManager.h"
#include "src/SdStorage.h"
#include "src/CameraManager.h"
#include "src/CaptureService.h"
#include "src/WebServerManager.h"

SettingsManager settings;
SdStorage storage;
CameraManager camera;
CaptureService capture;
WebServerManager web;

void setup() {
  Serial.begin(115200);
  delay(200);

  settings.begin();
  storage.begin();
  camera.begin(settings);
  capture.begin(settings, storage, camera);

  pinMode(PIN_BOOT_BUTTON, INPUT_PULLUP);
  uint32_t t0 = millis();
  bool syncMode = false;
  while (millis() - t0 < 3000) {
    if (digitalRead(PIN_BOOT_BUTTON) == LOW) {
      syncMode = true;
      break;
    }
    delay(10);
  }

  if (syncMode) {
    web.begin(settings, storage, capture);
    Serial.println("Sync mode enabled (AP + mDNS camera.local)");
  } else {
    Serial.println("Capture-only mode (Wi-Fi disabled)");
  }
}

void loop() {
  capture.loop();
  web.loop();
}
