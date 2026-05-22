#pragma once

#include <Arduino.h>

#define FW_VERSION "1.1.0"
#define PIN_BOOT_BUTTON 0
#define PIN_FLASH_LED 4

#define SD_CS_PIN 13
#define SD_MOSI 15
#define SD_MISO 2
#define SD_SCK 14

#define META_FILE "/meta.csv"
#define LAST_ID_FILE "/last_id.txt"

#define DEFAULT_BLE_NAME "HybridCam-ESP32"
#define BLE_MTU_TARGET 247
#define DEFAULT_CHUNK_SIZE 180
