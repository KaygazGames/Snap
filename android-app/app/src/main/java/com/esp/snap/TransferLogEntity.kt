package com.esp.snap

import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "transfer_logs")
data class TransferLogEntity(
    @PrimaryKey(autoGenerate = true) val id: Long = 0,
    val photoId: Long,
    val status: String,
    val retries: Int,
    val ts: Long = System.currentTimeMillis()
)
