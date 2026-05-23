#include "WebServerManager.h"
#include "AppConfig.h"
#include <WiFi.h>
#include <ESPmDNS.h>

void WebServerManager::begin(SettingsManager& settings, SdStorage& st, CaptureService& cs) {
  (void)settings;
  storage = &st;
  capture = &cs;

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_GW, AP_MASK);
  WiFi.softAP(AP_SSID, AP_PASS);
  MDNS.begin("camera");

  routes();
  server.begin();
  enabled = true;
}

void WebServerManager::routes() {
  server.on("/", HTTP_GET, [this]() {
    String html = "<html><body><h1>HybridCam</h1><button onclick=\"fetch('/snap',{method:'POST'}).then(r=>r.text()).then(alert)\">Snap</button><br/><a href='/meta'>meta</a></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/snap", HTTP_POST, [this]() {
    String p; uint32_t id;
    if (capture->snapNow(p, id)) server.send(200, "application/json", "{\"ok\":true,\"id\":"+String(id)+",\"file\":\""+p+"\"}");
    else server.send(500, "application/json", "{\"ok\":false}");
  });

  server.on("/latest", HTTP_GET, [this]() {
    server.send(200, "application/json", "{\"latest\":"+String(capture->latestId())+"}");
  });

  server.on("/meta", HTTP_GET, [this]() {
    server.send(200, "text/plain", storage->readAll(META_FILE));
  });

  server.on("/img", HTTP_GET, [this]() {
    if (!server.hasArg("id")) { server.send(400, "text/plain", "missing id"); return; }
    uint32_t id = server.arg("id").toInt();
    char f[32]; snprintf(f, sizeof(f), "/IMG_%06lu.jpg", (unsigned long)id);
    File file = storage->openRead(String(f));
    if (!file) { server.send(404, "text/plain", "not found"); return; }
    server.streamFile(file, "image/jpeg");
    file.close();
  });
}

void WebServerManager::loop() {
  if (enabled) server.handleClient();
}
