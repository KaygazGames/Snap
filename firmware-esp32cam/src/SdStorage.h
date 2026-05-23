#pragma once
#include <Arduino.h>
#include <FS.h>
#include <SD_MMC.h>

class SdStorage {
public:
  bool begin();
  bool writeBytes(const String& path, const uint8_t* data, size_t len);
  bool renameFile(const String& from, const String& to);
  bool exists(const String& path);
  String readText(const String& path);
  bool writeText(const String& path, const String& content);
  bool appendLine(const String& path, const String& line);
  File openRead(const String& path);
  uint64_t cardSize();
  uint64_t usedBytes();
};
