#pragma once
#include <Arduino.h>
#include <Preferences.h>

struct Settings {
  uint8_t jpegQuality;
  uint8_t frameSize;
};

class SettingsManager {
public:
  bool begin();
  const Settings& get() const { return s; }
private:
  Preferences prefs;
  Settings s;
};
