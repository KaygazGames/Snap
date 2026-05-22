package com.esp.snap

data class DeviceStatus(
    val connected: Boolean,
    val sdReady: Boolean,
    val batteryPercent: Int,
    val latestId: Long
)
