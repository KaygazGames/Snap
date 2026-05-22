#include "SdStorage.h"

bool SdStorage::begin() {
  return SD_MMC.begin("/sdcard", true);
}
bool SdStorage::writeBytes(const String& path, const uint8_t* data, size_t len) {
  File f = SD_MMC.open(path, FILE_WRITE);
  if (!f) return false;
  size_t w = f.write(data, len);
  f.flush();
  f.close();
  return w == len;
}
bool SdStorage::renameFile(const String& from, const String& to) { return SD_MMC.rename(from, to); }
bool SdStorage::exists(const String& path) { return SD_MMC.exists(path); }
String SdStorage::readText(const String& path) {
  File f = SD_MMC.open(path, FILE_READ); if (!f) return "";
  String out = f.readString(); f.close(); return out;
}
bool SdStorage::writeText(const String& path, const String& content) {
  File f = SD_MMC.open(path, FILE_WRITE); if (!f) return false;
  auto w = f.print(content); f.flush(); f.close(); return w == content.length();
}
bool SdStorage::appendLine(const String& path, const String& line) {
  File f = SD_MMC.open(path, FILE_APPEND); if (!f) return false;
  auto w = f.println(line); f.flush(); f.close(); return w > 0;
}
File SdStorage::openRead(const String& path) { return SD_MMC.open(path, FILE_READ); }
uint64_t SdStorage::cardSize() { return SD_MMC.cardSize(); }
uint64_t SdStorage::usedBytes() { return SD_MMC.usedBytes(); }
