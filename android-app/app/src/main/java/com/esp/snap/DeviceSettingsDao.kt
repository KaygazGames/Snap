package com.esp.snap

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query

@Dao
interface DeviceSettingsDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun upsert(entity: DeviceSettingsEntity)

    @Query("SELECT * FROM device_settings")
    suspend fun all(): List<DeviceSettingsEntity>
}
