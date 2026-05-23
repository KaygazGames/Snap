package com.esp.snap

import java.nio.ByteBuffer
import java.nio.ByteOrder

object BleProtocol {
    const val SOF: Byte = 0xA5.toByte()
    const val VER: Byte = 0x01

    const val GET_LATEST_ID: Byte = 0x01
    const val GET_METADATA: Byte = 0x02
    const val GET_HASH: Byte = 0x03
    const val START_TRANSFER: Byte = 0x04
    const val NEXT_CHUNK: Byte = 0x05
    const val END_TRANSFER: Byte = 0x06
    const val SET_SETTING: Byte = 0x07
    const val GET_SETTING: Byte = 0x08
    const val SAVE_SETTINGS: Byte = 0x09
    const val GET_STORAGE_INFO: Byte = 0x0A

    fun frame(cmd: Byte, reqId: Byte, payload: ByteArray = byteArrayOf()): ByteArray {
        val len = payload.size
        val head = byteArrayOf(SOF, VER, cmd, reqId, (len and 0xFF).toByte(), ((len shr 8) and 0xFF).toByte())
        val noCrc = head + payload
        val crc = crc16(noCrc)
        return noCrc + byteArrayOf((crc and 0xFF).toByte(), ((crc shr 8) and 0xFF).toByte())
    }

    private fun crc16(data: ByteArray): Int {
        var crc = 0xFFFF
        for (b in data) {
            crc = crc xor (b.toInt() and 0xFF)
            repeat(8) {
                crc = if ((crc and 1) != 0) (crc shr 1) xor 0xA001 else crc shr 1
            }
        }
        return crc and 0xFFFF
    }

    fun longToLe(v: Long): ByteArray = ByteBuffer.allocate(8).order(ByteOrder.LITTLE_ENDIAN).putLong(v).array()
}
