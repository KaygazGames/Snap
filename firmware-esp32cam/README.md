# HybridCam ESP32-CAM Web Firmware

- Board: AI Thinker ESP32-CAM + OV2640
- Storage: internal microSD only
- No BLE, no Android app

## Boot behavior
- First 3 seconds after boot:
  - If BOOT (GPIO0) is pressed: enable Sync Mode (AP web server + mDNS `camera.local`)
  - If not pressed: Wi-Fi never starts

## AP settings
- SSID: `HybridCam`
- Password: `hybridcam123`
- Hostname: `camera.local`

## Endpoints
- `GET /` web UI
- `POST /snap` capture now
- `GET /latest`
- `GET /meta`
- `GET /img?id=123`
