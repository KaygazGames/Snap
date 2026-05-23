#pragma once
#include <Arduino.h>
#include <SD_MMC.h>

class SdStorage {
public:
  bool begin();
  bool savePhotoAtomic(uint32_t id, const uint8_t* data, size_t len, String& finalPath);
  String readAll(const String& path);
  bool appendLine(const String& path, const String& line);
  bool writeText(const String& path, const String& text);
  bool exists(const String& path);
  String readText(const String& path);
  uint32_t loadLastId();
  bool saveLastId(uint32_t id);
  File openRead(const String& path);
};
