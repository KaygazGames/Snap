package com.esp.snap

import android.content.Context

class SyncManager(
    private val repo: EspRepository,
    private val db: AppDatabase
) {
    suspend fun manualSync(context: Context): String {
        val latest = repo.getLatestId()
        val localMax = db.photoDao().maxPhotoId() ?: 0L
        if (latest <= localMax) return "Up to date"

        var downloaded = 0
        for (id in (localMax + 1)..latest) {
            val meta = repo.getMetadata(id)
            val bytes = repo.downloadPhotoBytes(id)
            val hash = HashUtils.sha256Hex(bytes)
            if (!hash.equals(meta.sha256, ignoreCase = true)) {
                db.transferLogDao().insert(TransferLogEntity(photoId = id, status = "HASH_MISMATCH", retries = 1))
                continue
            }
            val uri = MediaStoreWriter.saveJpeg(context, meta.filename, bytes)
            db.photoDao().insert(
                PhotoEntity(
                    photoId = id,
                    filename = meta.filename,
                    sha256 = hash,
                    timestamp = meta.timestamp,
                    localPath = uri.toString(),
                    synced = true,
                    verified = true,
                    fileSize = bytes.size.toLong()
                )
            )
            db.transferLogDao().insert(TransferLogEntity(photoId = id, status = "OK", retries = 0))
            downloaded++
        }
        return "Manual sync complete: $downloaded downloaded"
    }
}
