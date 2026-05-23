#include "SdStorage.h"
#include "AppConfig.h"

bool SdStorage::begin() {
  return SD_MMC.begin("/sdcard", true);
}

bool SdStorage::savePhotoAtomic(uint32_t id, const uint8_t* data, size_t len, String& finalPath) {
  char t[32], f[32];
  snprintf(t, sizeof(t), "/TMP_%06lu.jpg", (unsigned long)id);
  snprintf(f, sizeof(f), "/IMG_%06lu.jpg", (unsigned long)id);
  File tf = SD_MMC.open(t, FILE_WRITE);
  if (!tf) return false;
  size_t w = tf.write(data, len);
  tf.flush(); tf.close();
  if (w != len) { SD_MMC.remove(t); return false; }
  if (SD_MMC.exists(f)) SD_MMC.remove(f);
  if (!SD_MMC.rename(t, f)) { SD_MMC.remove(t); return false; }
  finalPath = String(f);
  return true;
}

String SdStorage::readAll(const String& path) { File f = SD_MMC.open(path); if(!f)return ""; String s=f.readString(); f.close(); return s; }
bool SdStorage::appendLine(const String& path, const String& line) { File f=SD_MMC.open(path, FILE_APPEND); if(!f)return false; size_t w=f.println(line); f.flush(); f.close(); return w>0; }
bool SdStorage::writeText(const String& path, const String& text) { if(SD_MMC.exists(path))SD_MMC.remove(path); File f=SD_MMC.open(path, FILE_WRITE); if(!f)return false; size_t w=f.print(text); f.flush(); f.close(); return w==text.length(); }
bool SdStorage::exists(const String& path){ return SD_MMC.exists(path); }
String SdStorage::readText(const String& path){ return readAll(path); }
uint32_t SdStorage::loadLastId(){ String v=readAll(LAST_ID_FILE); return v.length()?v.toInt():0; }
bool SdStorage::saveLastId(uint32_t id){ return writeText(LAST_ID_FILE, String(id)); }
File SdStorage::openRead(const String& path){ return SD_MMC.open(path, FILE_READ); }
