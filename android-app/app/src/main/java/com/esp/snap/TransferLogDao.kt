package com.esp.snap

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query

@Dao
interface TransferLogDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insert(log: TransferLogEntity)

    @Query("SELECT * FROM transfer_logs ORDER BY ts DESC")
    suspend fun all(): List<TransferLogEntity>
}
