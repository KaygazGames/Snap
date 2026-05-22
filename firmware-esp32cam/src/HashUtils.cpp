#include "HashUtils.h"
#include <SD_MMC.h>
#include "mbedtls/sha256.h"

String HashUtils::sha256File(const String& path) {
  File f = SD_MMC.open(path, FILE_READ);
  if (!f) return "";
  mbedtls_sha256_context ctx;
  mbedtls_sha256_init(&ctx);
  mbedtls_sha256_starts(&ctx, 0);
  uint8_t buf[1024];
  while (f.available()) {
    size_t n = f.read(buf, sizeof(buf));
    if (!n) break;
    mbedtls_sha256_update(&ctx, buf, n);
  }
  unsigned char out[32];
  mbedtls_sha256_finish(&ctx, out);
  mbedtls_sha256_free(&ctx);
  f.close();
  char hex[65];
  for (int i=0;i<32;i++) sprintf(hex + i*2, "%02x", out[i]);
  hex[64]='\0';
  return String(hex);
}
