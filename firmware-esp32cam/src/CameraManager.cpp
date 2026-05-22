#include "CameraManager.h"

bool CameraManager::begin(SettingsManager& s) {
  settings = &s;
  camera_config_t c{};
  c.ledc_channel = LEDC_CHANNEL_0;
  c.ledc_timer = LEDC_TIMER_0;
  c.pin_d0 = 5; c.pin_d1 = 18; c.pin_d2 = 19; c.pin_d3 = 21; c.pin_d4 = 36; c.pin_d5 = 39; c.pin_d6 = 34; c.pin_d7 = 35;
  c.pin_xclk = 0; c.pin_pclk = 22; c.pin_vsync = 25; c.pin_href = 23; c.pin_sccb_sda = 26; c.pin_sccb_scl = 27;
  c.pin_pwdn = 32; c.pin_reset = -1; c.xclk_freq_hz = 20000000; c.pixel_format = PIXFORMAT_JPEG;
  c.frame_size = (framesize_t)settings->get().frameSize; c.jpeg_quality = settings->get().jpegQuality; c.fb_count = 2;
  if (esp_camera_init(&c) != ESP_OK) return false;
  sensor_t* sen = esp_camera_sensor_get();
  sen->set_brightness(sen, settings->get().brightness);
  sen->set_contrast(sen, settings->get().contrast);
  sen->set_saturation(sen, settings->get().saturation);
  sen->set_sharpness(sen, settings->get().sharpness);
  sen->set_whitebal(sen, settings->get().awb);
  sen->set_gain_ctrl(sen, settings->get().agc);
  sen->set_exposure_ctrl(sen, settings->get().aec);
  return true;
}
camera_fb_t* CameraManager::captureWarmupAndGetFrame() {
  camera_fb_t* first = esp_camera_fb_get();
  if (first) esp_camera_fb_return(first);
  return esp_camera_fb_get();
}
void CameraManager::release(camera_fb_t* fb) { if (fb) esp_camera_fb_return(fb); }
uint8_t CameraManager::estimateLuma(const uint8_t* data, size_t len) const {
  if (!data || len < 4) return 128;
  uint32_t sum = 0; size_t n = min((size_t)64, len);
  for (size_t i=0;i<n;i++) sum += data[i];
  return (uint8_t)(sum / n);
}
