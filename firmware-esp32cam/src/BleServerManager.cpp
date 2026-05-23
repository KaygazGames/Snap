#include "BleServerManager.h"
#include <NimBLEDevice.h>
#include "AppConfig.h"

static NimBLECharacteristic* gRsp = nullptr;
static CaptureService* gCapture = nullptr;

class CmdCallbacks : public NimBLECharacteristicCallbacks {
  void handleWrite(NimBLECharacteristic* c) {
    std::string v = c->getValue();
    if (v.empty()) return;
    uint8_t cmd = (uint8_t)v[0];

    // Minimal command set:
    // 0x20 = trigger capture, response: 0x00 fail / 0x01 ok
    // 0x01 = get latest id, response: little-endian uint32
    if (cmd == 0x20) {
      uint8_t ok = (gCapture && gCapture->triggerCapture()) ? 1 : 0;
      gRsp->setValue(&ok, 1);
      gRsp->notify();
      return;
    }

    if (cmd == 0x01) {
      uint8_t zero[4] = {0,0,0,0};
      gRsp->setValue(zero, 4);
      gRsp->notify();
      return;
    }
  }

  // NimBLE-Arduino callback signatures vary across versions.
  // Keep both overloads so this compiles with old/new releases.
  void onWrite(NimBLECharacteristic* c) {
    handleWrite(c);
  }

  void onWrite(NimBLECharacteristic* c, NimBLEConnInfo& connInfo) {
    (void)connInfo;
    handleWrite(c);
  }
};

void BleServerManager::begin(SettingsManager& settings, SdStorage& storage, MetadataStore& metadata, FlashController& flash, CameraManager& camera, CaptureService& capture) {
  captureService = &capture;
  gCapture = &capture;

  NimBLEDevice::init(settings.get().bleName);
  NimBLEDevice::setMTU(BLE_MTU_TARGET);
  NimBLEServer* server = NimBLEDevice::createServer();
  auto* svc = server->createService("0000A100-0000-1000-8000-00805F9B34FB");

  auto* cmd = svc->createCharacteristic("0000A101-0000-1000-8000-00805F9B34FB", NIMBLE_PROPERTY::WRITE);
  gRsp = svc->createCharacteristic("0000A102-0000-1000-8000-00805F9B34FB", NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::READ);
  cmd->setCallbacks(new CmdCallbacks());

  svc->start();
  NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
  adv->addServiceUUID(svc->getUUID());
  adv->start();
}

void BleServerManager::loop() {
  delay(2);
}
