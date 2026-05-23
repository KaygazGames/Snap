#pragma once
#include <Arduino.h>

#define FW_VERSION "2.0.0"
#define PIN_BOOT_BUTTON 0

#define AP_SSID "HybridCam"
#define AP_PASS "hybridcam123"
#define AP_IP IPAddress(192,168,4,1)
#define AP_GW IPAddress(192,168,4,1)
#define AP_MASK IPAddress(255,255,255,0)

#define META_FILE "/meta.csv"
#define LAST_ID_FILE "/last_id.txt"
