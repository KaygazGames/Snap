# HybridCam ESP32-CAM Firmware (AI Thinker + OV2640)

This firmware targets **AI Thinker ESP32-CAM with OV2640 only**.

## Features
- Capture on BOOT button press with debounce.
- SD-only image persistence using temp file then atomic rename.
- Sequential image IDs (`IMG_000001.jpg`, etc.) tracked in `/last_id.txt`.
- SHA-256 file hashing after write.
- Metadata journal in `/meta.csv`.
- Persistent camera/flash/transfer settings in NVS via `Preferences`.
- BLE server bootstrap (NimBLE) for control plane integration.

## Arduino Libraries
- `esp32` core for Arduino
- `NimBLE-Arduino`

## Build notes
- Board: `AI Thinker ESP32-CAM`
- Partition: huge app / no OTA recommended for camera + BLE.
- Ensure a formatted microSD card is installed before boot.
