# HybridCam ESP32-CAM Firmware (AI Thinker + OV2640)

This firmware targets **AI Thinker ESP32-CAM with OV2640 only** and writes photos **only to the onboard microSD slot**.

## Important hardware note
- On AI Thinker ESP32-CAM, GPIO0 is camera XCLK, so the onboard BOOT key cannot be a reliable runtime shutter.
- This firmware uses:
  - BLE command `0x20` as the primary shutter trigger.
  - Optional external button on `GPIO12` (`PIN_SHUTTER_BUTTON`).

## Features
- Capture trigger via BLE and optional button with debounce.
- SD-only image persistence using temp file then atomic rename.
- Sequential IDs (`IMG_000001.jpg`, etc.) tracked in `/last_id.txt`.
- SHA-256 file hashing after write.
- Metadata journal in `/meta.csv`.
- Persistent camera/flash/transfer settings in NVS via `Preferences`.
- NimBLE control service for Android integration.

## Arduino Libraries
- `esp32` core for Arduino
- `NimBLE-Arduino`

## Build notes
- Board: `AI Thinker ESP32-CAM`
- Partition: huge app / no OTA recommended for camera + BLE.
- Ensure a formatted microSD card is installed before boot.
