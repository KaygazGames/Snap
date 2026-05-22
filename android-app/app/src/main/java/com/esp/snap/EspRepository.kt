package com.esp.snap

import java.nio.ByteBuffer
import java.nio.ByteOrder

class EspRepository(
    private val ble: BleManager,
    private val db: AppDatabase
) {
    suspend fun scanAndConnectFirstSnap(): Boolean = ble.scanAndConnectFirst("HybridCam")

    suspend fun getLatestId(): Long {
        val rsp = ble.request(BleProtocol.GET_LATEST_ID)
        if (rsp.size < 8) return 0L
        return ByteBuffer.wrap(rsp.takeLast(8).toByteArray()).order(ByteOrder.LITTLE_ENDIAN).long
    }

    suspend fun getStatusText(): String {
        val rsp = ble.request(BleProtocol.GET_STORAGE_INFO)
        return "Storage response bytes=${rsp.size}"
    }

    suspend fun getMetadata(id: Long): CameraMetadata {
        val rsp = ble.request(BleProtocol.GET_METADATA, BleProtocol.longToLe(id))
        val sha = rsp.takeLast(64).toByteArray().decodeToString().trim()
        return CameraMetadata(
            photoId = id,
            filename = "IMG_%06d.jpg".format(id),
            sha256 = sha,
            timestamp = System.currentTimeMillis().toString(),
            fileSize = 0L
        )
    }

    suspend fun downloadPhotoBytes(id: Long): ByteArray {
        ble.request(BleProtocol.START_TRANSFER, BleProtocol.longToLe(id))
        val out = ArrayList<Byte>()
        while (true) {
            val chunk = ble.request(BleProtocol.NEXT_CHUNK)
            if (chunk.isEmpty()) break
            out.addAll(chunk.toList())
            if (chunk.size < 180) break
        }
        ble.request(BleProtocol.END_TRANSFER)
        return out.toByteArray()
    }
}
