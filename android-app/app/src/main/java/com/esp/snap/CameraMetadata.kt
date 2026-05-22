package com.esp.snap

data class CameraMetadata(
    val photoId: Long,
    val filename: String,
    val sha256: String,
    val timestamp: String,
    val fileSize: Long
)
