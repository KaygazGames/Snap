package com.esp.snap

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query

@Dao
interface PhotoDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(photo: PhotoEntity)

    @Query("SELECT MAX(photo_id) FROM photos")
    suspend fun maxPhotoId(): Long?

    @Query("SELECT * FROM photos ORDER BY photo_id DESC")
    suspend fun all(): List<PhotoEntity>
}
