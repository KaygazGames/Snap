#pragma once
#include <Arduino.h>
#include <WebServer.h>
#include "SettingsManager.h"
#include "SdStorage.h"
#include "CaptureService.h"

class WebServerManager {
public:
  void begin(SettingsManager& settings, SdStorage& storage, CaptureService& capture);
  void loop();
private:
  bool enabled{false};
  WebServer server{80};
  SdStorage* storage{nullptr};
  CaptureService* capture{nullptr};
  void routes();
};
