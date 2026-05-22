package com.esp.snap

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.Index
import androidx.room.PrimaryKey

@Entity(tableName = "photos", indices = [Index(value = ["photo_id"], unique = true)])
data class PhotoEntity(
    @PrimaryKey(autoGenerate = true) val id: Long = 0,
    @ColumnInfo(name = "photo_id") val photoId: Long,
    val filename: String,
    val sha256: String,
    val timestamp: String,
    @ColumnInfo(name = "local_path") val localPath: String,
    val synced: Boolean,
    val verified: Boolean,
    @ColumnInfo(name = "file_size") val fileSize: Long
)
