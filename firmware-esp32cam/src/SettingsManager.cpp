#include "SettingsManager.h"

bool SettingsManager::begin() {
  prefs.begin("hybridcam", false);
  s.jpegQuality = prefs.getUChar("jpegQ", 10);
  s.frameSize = prefs.getUChar("frame", 10);
  return true;
}
