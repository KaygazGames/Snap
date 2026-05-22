#include "MetadataStore.h"
#include "AppConfig.h"

bool MetadataStore::begin(SdStorage& storage) {
  s = &storage;
  if (!s->exists(META_FILE)) s->writeText(META_FILE, "id,filename,sha256,size,timestamp\n");
  String t = s->readText(LAST_ID_FILE);
  lastId = t.length() ? t.toInt() : 0;
  return true;
}
uint32_t MetadataStore::nextIdAtomic() {
  lastId += 1;
  s->writeText(LAST_ID_FILE, String(lastId));
  return lastId;
}
bool MetadataStore::append(const PhotoMeta& m) {
  String line = String(m.id)+","+m.filename+","+m.sha256+","+String(m.size)+","+m.ts;
  return s->appendLine(META_FILE, line);
}
String MetadataStore::filenameFor(uint32_t id) {
  char buf[32]; snprintf(buf, sizeof(buf), "/IMG_%06lu.jpg", (unsigned long)id); return String(buf);
}
bool MetadataStore::getMetadata(uint32_t id, PhotoMeta& out) {
  File f = s->openRead(META_FILE); if (!f) return false;
  while (f.available()) {
    String line = f.readStringUntil('\n');
    if (line.startsWith(String(id)+",")) {
      int p1=line.indexOf(','); int p2=line.indexOf(',',p1+1); int p3=line.indexOf(',',p2+1); int p4=line.indexOf(',',p3+1);
      out.id=id; out.filename=line.substring(p1+1,p2); out.sha256=line.substring(p2+1,p3); out.size=line.substring(p3+1,p4).toInt(); out.ts=line.substring(p4+1);
      f.close(); return true;
    }
  }
  f.close(); return false;
}
