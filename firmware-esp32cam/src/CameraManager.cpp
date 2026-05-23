#include "CameraManager.h"

bool CameraManager::begin(SettingsManager& settings) {
  camera_config_t c{};
  c.ledc_channel = LEDC_CHANNEL_0; c.ledc_timer = LEDC_TIMER_0;
  c.pin_d0=5;c.pin_d1=18;c.pin_d2=19;c.pin_d3=21;c.pin_d4=36;c.pin_d5=39;c.pin_d6=34;c.pin_d7=35;
  c.pin_xclk=0;c.pin_pclk=22;c.pin_vsync=25;c.pin_href=23;c.pin_sscb_sda=26;c.pin_sscb_scl=27;c.pin_pwdn=32;c.pin_reset=-1;
  c.xclk_freq_hz=20000000; c.pixel_format=PIXFORMAT_JPEG;
  c.frame_size=(framesize_t)settings.get().frameSize; c.jpeg_quality=settings.get().jpegQuality;
  c.fb_location=CAMERA_FB_IN_PSRAM; c.fb_count=2; c.grab_mode=CAMERA_GRAB_LATEST;
  if (esp_camera_init(&c) != ESP_OK) return false;
  camera_fb_t* warm=esp_camera_fb_get(); if(warm)esp_camera_fb_return(warm);
  return true;
}

camera_fb_t* CameraManager::capture(){ camera_fb_t* throwaway=esp_camera_fb_get(); if(throwaway)esp_camera_fb_return(throwaway); return esp_camera_fb_get(); }
void CameraManager::release(camera_fb_t* fb){ if(fb)esp_camera_fb_return(fb); }
