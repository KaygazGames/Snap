#include "CaptureService.h"
#include "HashUtils.h"

void CaptureService::begin(SettingsManager& s, SdStorage& st, MetadataStore& m, CameraManager& c, FlashController& f, uint8_t buttonPin) {
  settings=&s; storage=&st; metadata=&m; camera=&c; flash=&f; pin=buttonPin;
  pinMode(pin, INPUT_PULLUP);
}

bool CaptureService::shouldCapture() {
  bool reading = digitalRead(pin);
  if (reading != prevState) lastDebounceMs = millis();
  if ((millis() - lastDebounceMs) > 40 && prevState == HIGH && reading == LOW) {
    prevState = reading;
    return true;
  }
  prevState = reading;
  return false;
}

bool CaptureService::capture() {
  uint32_t id = metadata->nextIdAtomic();
  char tmpName[32], finalName[32];
  snprintf(tmpName, sizeof(tmpName), "/TMP_%06lu.jpg", (unsigned long)id);
  snprintf(finalName, sizeof(finalName), "/IMG_%06lu.jpg", (unsigned long)id);

  camera_fb_t* fb = camera->captureWarmupAndGetFrame();
  if (!fb || fb->len < 1000) { camera->release(fb); return false; }

  if (flash->shouldAutoFire(camera->estimateLuma(fb->buf, fb->len))) {
    flash->pulseForCapture();
    camera->release(fb);
    fb = camera->captureWarmupAndGetFrame();
    if (!fb) return false;
  }

  if (!storage->writeBytes(tmpName, fb->buf, fb->len)) { camera->release(fb); return false; }

  String sha = HashUtils::sha256File(tmpName);
  if (sha.length() != 64) {
    camera->release(fb);
    return false;
  }

  if (!storage->renameFile(tmpName, finalName)) { camera->release(fb); return false; }

  PhotoMeta m{ id, String(finalName), sha, (uint32_t)fb->len, String(millis()) };
  bool metaOk = metadata->append(m);
  camera->release(fb);
  return metaOk;
}

bool CaptureService::triggerCapture() { return capture(); }

void CaptureService::loop() {
  if (shouldCapture()) capture();
}
