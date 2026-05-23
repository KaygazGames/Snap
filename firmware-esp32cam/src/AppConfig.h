#pragma once

#include <Arduino.h>

#define FW_VERSION "1.2.0"

// IMPORTANT:
// AI Thinker ESP32-CAM uses GPIO0 for camera XCLK, so onboard BOOT key cannot be used
// as a runtime shutter input while camera is active. Keep BLE capture as primary trigger.
#define PIN_SHUTTER_BUTTON 12   // optional external shutter button
#define PIN_FLASH_LED 4

#define META_FILE "/meta.csv"
#define LAST_ID_FILE "/last_id.txt"

#define DEFAULT_BLE_NAME "HybridCam-ESP32"
#define BLE_MTU_TARGET 247
#define DEFAULT_CHUNK_SIZE 180
