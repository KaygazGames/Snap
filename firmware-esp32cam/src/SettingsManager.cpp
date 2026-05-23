#include "SettingsManager.h"
#include "AppConfig.h"
#include <string.h>

bool SettingsManager::begin() {
  prefs.begin("hybridcam", false);
  defaults();
  String bn = prefs.getString("bleName", DEFAULT_BLE_NAME);
  bn.toCharArray(s.bleName, sizeof(s.bleName));
  s.jpegQuality = prefs.getUChar("jpegQ", s.jpegQuality);
  s.frameSize = prefs.getUChar("frame", s.frameSize);
  s.flashMode = (FlashMode)prefs.getUChar("fMode", s.flashMode);
  s.flashIntensity = prefs.getUChar("fInt", s.flashIntensity);
  s.autoFlashThreshold = prefs.getUChar("fThr", s.autoFlashThreshold);
  s.chunkSize = prefs.getUShort("chunk", s.chunkSize);
  s.retryCount = prefs.getUChar("retry", s.retryCount);
  validate();
  return true;
}

void SettingsManager::defaults() {
  strncpy(s.bleName, DEFAULT_BLE_NAME, sizeof(s.bleName));
  s.jpegQuality = 10;
  s.frameSize = 10;
  s.brightness = 1; s.contrast = 1; s.saturation = 0; s.sharpness = 1;
  s.awb = true; s.agc = true; s.aec = true;
  s.flashMode = FLASH_AUTO; s.flashIntensity = 140; s.autoFlashThreshold = 90;
  s.chunkSize = DEFAULT_CHUNK_SIZE; s.retryCount = 5;
}

void SettingsManager::validate() {
  if (s.jpegQuality < 4) s.jpegQuality = 4;
  if (s.jpegQuality > 63) s.jpegQuality = 63;
  if (s.chunkSize < 64) s.chunkSize = 64;
  if (s.chunkSize > 220) s.chunkSize = 220;
}

bool SettingsManager::update(const String& key, const String& value) {
  if (key == "flash_mode") s.flashMode = (FlashMode)value.toInt();
  else if (key == "flash_intensity") s.flashIntensity = (uint8_t)value.toInt();
  else if (key == "jpeg_quality") s.jpegQuality = (uint8_t)value.toInt();
  else if (key == "chunk_size") s.chunkSize = (uint16_t)value.toInt();
  else return false;
  validate();
  return true;
}

bool SettingsManager::save() {
  prefs.putString("bleName", s.bleName);
  prefs.putUChar("jpegQ", s.jpegQuality);
  prefs.putUChar("frame", s.frameSize);
  prefs.putUChar("fMode", (uint8_t)s.flashMode);
  prefs.putUChar("fInt", s.flashIntensity);
  prefs.putUChar("fThr", s.autoFlashThreshold);
  prefs.putUShort("chunk", s.chunkSize);
  prefs.putUChar("retry", s.retryCount);
  return true;
}

void SettingsManager::factoryReset() {
  defaults();
  save();
}
