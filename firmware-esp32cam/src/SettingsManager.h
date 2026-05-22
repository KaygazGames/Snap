#pragma once

#include <Arduino.h>
#include <Preferences.h>

enum FlashMode : uint8_t { FLASH_OFF=0, FLASH_ON=1, FLASH_AUTO=2, FLASH_TORCH=3 };

struct Settings {
  char bleName[32];
  uint8_t jpegQuality;
  uint8_t frameSize;
  int8_t brightness;
  int8_t contrast;
  int8_t saturation;
  int8_t sharpness;
  bool awb;
  bool agc;
  bool aec;
  FlashMode flashMode;
  uint8_t flashIntensity;
  uint8_t autoFlashThreshold;
  uint16_t chunkSize;
  uint8_t retryCount;
};

class SettingsManager {
public:
  bool begin();
  const Settings& get() const { return s; }
  bool update(const String& key, const String& value);
  bool save();
  void factoryReset();
private:
  Preferences prefs;
  Settings s;
  void defaults();
  void validate();
};
