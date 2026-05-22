#pragma once
#include <Arduino.h>

class HashUtils {
public:
  static String sha256File(const String& path);
};
