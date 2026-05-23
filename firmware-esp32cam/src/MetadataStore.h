#pragma once
#include <Arduino.h>
#include "SdStorage.h"

struct PhotoMeta { uint32_t id; String filename; String sha256; uint32_t size; String ts; };

class MetadataStore {
public:
  bool begin(SdStorage& storage);
  uint32_t nextIdAtomic();
  bool append(const PhotoMeta& m);
  String filenameFor(uint32_t id);
  bool getMetadata(uint32_t id, PhotoMeta& out);
  uint32_t latestId() const { return lastId; }
private:
  SdStorage* s {nullptr};
  uint32_t lastId {0};
};
