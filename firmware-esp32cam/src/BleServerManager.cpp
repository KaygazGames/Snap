#include "BleServerManager.h"
#include <NimBLEDevice.h>

void BleServerManager::begin(SettingsManager& settings, SdStorage& storage, MetadataStore& metadata, FlashController& flash, CameraManager& camera) {
  NimBLEDevice::init(settings.get().bleName);
  NimBLEServer* server = NimBLEDevice::createServer();
  auto* svc = server->createService("0000A100-0000-1000-8000-00805F9B34FB");
  svc->createCharacteristic("0000A101-0000-1000-8000-00805F9B34FB", NIMBLE_PROPERTY::WRITE);
  svc->createCharacteristic("0000A102-0000-1000-8000-00805F9B34FB", NIMBLE_PROPERTY::NOTIFY);
  svc->start();
  NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
  adv->addServiceUUID(svc->getUUID());
  adv->start();
}

void BleServerManager::loop() {
  delay(2);
}
