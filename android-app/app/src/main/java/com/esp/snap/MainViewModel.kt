package com.esp.snap

import android.app.Application
import android.content.Context
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.launch

class MainViewModel(app: Application) : AndroidViewModel(app) {
    private val db = (app as HybridCamApp).db
    private val ble = BleManager(app.applicationContext)
    private val repo = EspRepository(ble, db)
    private val syncManager = SyncManager(repo, db)

    val uiState = MutableLiveData("Idle")

    fun scanAndConnect(context: Context) = viewModelScope.launch {
        uiState.value = "Scanning..."
        val ok = repo.scanAndConnectFirstSnap()
        uiState.value = if (ok) "Connected" else "Connect failed"
    }

    fun refreshStatus() = viewModelScope.launch {
        val st = repo.getStatusText()
        uiState.value = st
    }

    fun manualSync(context: Context) = viewModelScope.launch {
        uiState.value = "Manual sync running..."
        val result = syncManager.manualSync(context)
        uiState.value = result
    }
}
