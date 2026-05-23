# Prompt for Another AI: Build HybridCam on PlatformIO with Modern Web GUI

You are a senior embedded + full-stack engineer.

Build a **production-grade PlatformIO project** for **AI Thinker ESP32-CAM with OV2640 only** that behaves like a standalone hybrid digital camera and includes a **modern responsive web GUI**.

## Hard Requirements (Do Not Violate)
- Board must remain: **AI Thinker ESP32-CAM**
- Camera must remain: **OV2640**
- Storage must remain: **internal microSD slot on ESP32-CAM**
- All photos must be stored to SD card only (no SPIFFS photo storage)
- No BLE
- Wi-Fi only for AP sync mode
- Boot behavior:
  - During first 3 seconds after boot, if BOOT button (GPIO0) is pressed -> enable Sync Mode (Wi-Fi AP + web server + mDNS)
  - If not pressed -> Wi-Fi must stay OFF entirely
- mDNS hostname in sync mode: **camera.local**

---

## Deliverables
Provide complete, compile-ready source code (no pseudocode, no TODO placeholders):

1. `platformio.ini`
2. Full `src/` firmware implementation
3. Full `include/` headers
4. Full web app assets in `data/` (HTML/CSS/JS)
5. Persistent settings system (NVS Preferences)
6. SD metadata/index system
7. Camera capture + file integrity flow
8. README with flashing, uploadfs, and usage instructions

Use solid modular architecture and production naming.

---

## PlatformIO Constraints
- Framework: Arduino
- Platform: espressif32
- Board: `esp32cam` (AI Thinker pinmap explicitly configured in camera init)
- Include `ESP Async WebServer` + `AsyncTCP` + `ESPmDNS`
- Use LittleFS/SPIFFS only for web assets if needed, but **never for photos**
- Photos must be saved to `/IMG_XXXXXX.jpg` on SD_MMC

---

## Firmware Features

### 1) Capture Pipeline
Implement a robust capture service:
1. Debounce trigger
2. Capture frame (discard first unstable frame)
3. Validate frame length
4. Save to temp file `/TMP_XXXXXX.jpg`
5. Flush and close
6. Compute SHA-256 hash of saved temp file
7. Rename atomically to `/IMG_XXXXXX.jpg`
8. Append metadata to CSV index
9. Persist last-id

File naming must be strict sequential:
- `IMG_000001.jpg`, `IMG_000002.jpg`, ...

Prevent:
- duplicate IDs
- id rollback after reboot
- partial writes
- accidental overwrite

### 2) SD Index / Metadata
Maintain `meta.csv` with columns:
- id
- filename
- sha256
- file_size
- timestamp

Also maintain durable `last_id` record.

### 3) Camera Tuning (Indoor Priority)
Tune OV2640 for high quality indoor use:
- JPEG pixel format
- stable high-quality JPEG setting
- AWB enabled
- AEC enabled
- AGC enabled
- brightness/contrast/saturation/sharpness sane defaults
- PSRAM frame buffers configured for stability
- first-frame discard handling

### 4) Boot Sync Mode Logic
In `setup()`:
- Sample BOOT button for 3 seconds window
- If pressed: start AP + mDNS + web server
- Else: do not initialize Wi-Fi stack beyond what is unavoidable; ensure AP/server are not started

### 5) AP + mDNS
Sync mode only:
- AP SSID configurable via settings (default `HybridCam`)
- AP password configurable (default strong pass)
- mDNS hostname: `camera.local`

### 6) Settings Persistence (NVS)
Persist settings on-device:
- jpeg_quality
- frame_size
- shutter debounce
- AP SSID / password
- optional capture delay
- UI theme preference (if needed)

### 7) Web Server API
Implement JSON REST endpoints:
- `GET /api/status` -> sd state, latest id, image count, free/used bytes, sync mode flag
- `POST /api/capture` -> trigger capture now
- `GET /api/photos?from_id=...&limit=...`
- `GET /api/photo/:id` -> stream JPEG
- `GET /api/photo/:id/hash`
- `DELETE /api/photo/:id`
- `DELETE /api/photos` -> delete all with confirmation token
- `GET /api/settings`
- `POST /api/settings`
- `POST /api/reindex` -> rebuild metadata from SD scan

All responses should include explicit success/error fields.

### 8) Modern Web GUI (Important)
Create polished responsive GUI in `data/`:
- Dashboard cards: SD status, image count, latest ID, free space
- Capture button with feedback
- Gallery grid with thumbnails
- Photo detail modal (id, timestamp, size, hash)
- Settings panel (camera + AP)
- Maintenance panel (reindex, delete all)
- Toast notifications and loading states
- Dark modern style (clean typography, spacing, mobile-first)

Use vanilla JS or lightweight framework, but ensure no build step required unless clearly documented.

### 9) Reliability Requirements
- Strong error handling for SD absent/failure
- Timeouts and safe returns on capture failures
- Avoid blocking loops in web handlers
- Memory-conscious string usage
- Prevent crash on malformed query params

---

## Required Project Structure

```text
HybridCamPIO/
  platformio.ini
  include/
    AppConfig.h
    ...
  src/
    main.cpp
    CameraManager.cpp
    CaptureService.cpp
    SdStorage.cpp
    MetadataStore.cpp
    SettingsManager.cpp
    WebServerManager.cpp
    HashUtils.cpp
    ...
  data/
    index.html
    app.css
    app.js
  README.md
```

---

## Coding Standards
- Production-style comments explaining *why* decisions were made
- No pseudocode
- No stubbed methods
- Compile-ready on PlatformIO
- Keep modules cohesive and testable

---

## Final Output Format
Return:
1. Full file tree
2. Complete content of every file
3. Setup steps:
   - `pio run`
   - `pio run -t upload`
   - `pio run -t uploadfs`
   - serial monitor steps
4. Validation checklist for:
   - capture in non-sync mode
   - AP sync mode
   - web capture
   - gallery access
   - metadata/hash correctness

