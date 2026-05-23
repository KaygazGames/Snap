package com.esp.snap

import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "device_settings")
data class DeviceSettingsEntity(
    @PrimaryKey val key: String,
    val value: String
)
