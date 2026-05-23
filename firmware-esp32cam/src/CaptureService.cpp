#include "CaptureService.h"
#include "AppConfig.h"

void CaptureService::begin(SettingsManager& s, SdStorage& st, CameraManager& c) {
  settings = &s; storage = &st; camera = &c;
  if (!storage->openRead(META_FILE)) storage->writeText(META_FILE, "id,filename,size,timestamp\n");
  lastId = storage->loadLastId();
}

bool CaptureService::snapNow(String& outPath, uint32_t& outId) {
  uint32_t id = lastId + 1;
  camera_fb_t* fb = camera->capture();
  if (!fb || fb->len < 1000) { camera->release(fb); return false; }
  String path;
  if (!storage->savePhotoAtomic(id, fb->buf, fb->len, path)) { camera->release(fb); return false; }
  String line = String(id)+","+path+","+String((uint32_t)fb->len)+","+String((uint32_t)millis());
  if (!storage->appendLine(META_FILE, line)) { camera->release(fb); return false; }
  if (!storage->saveLastId(id)) { camera->release(fb); return false; }
  camera->release(fb);
  lastId = id;
  outPath = path;
  outId = id;
  return true;
}

void CaptureService::loop() {
  // runtime shutter only outside sync window; simple debounce
  if (digitalRead(PIN_BOOT_BUTTON) == LOW && millis() - lastPress > 500) {
    lastPress = millis();
    String p; uint32_t id;
    snapNow(p, id);
  }
}
