package com.esp.snap

import android.annotation.SuppressLint
import android.bluetooth.*
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanResult
import android.content.Context
import kotlinx.coroutines.CompletableDeferred
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.util.concurrent.atomic.AtomicReference

class BleManager(private val context: Context) {
    private val adapter: BluetoothAdapter? by lazy {
        (context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager).adapter
    }
    private val gattRef = AtomicReference<BluetoothGatt?>(null)
    private var rspDeferred = CompletableDeferred<ByteArray>()

    @SuppressLint("MissingPermission")
    suspend fun scanAndConnectFirst(namePrefix: String): Boolean = withContext(Dispatchers.IO) {
        val scanner = adapter?.bluetoothLeScanner ?: return@withContext false
        val found = CompletableDeferred<BluetoothDevice?>()

        val cb = object : ScanCallback() {
            override fun onScanResult(callbackType: Int, result: ScanResult) {
                val n = result.device.name ?: return
                if (n.startsWith(namePrefix) && !found.isCompleted) found.complete(result.device)
            }
        }
        scanner.startScan(cb)
        val dev = kotlin.runCatching { found.await() }.getOrNull()
        scanner.stopScan(cb)
        if (dev == null) return@withContext false

        val okConn = CompletableDeferred<Boolean>()
        val gatt = dev.connectGatt(context, false, object : BluetoothGattCallback() {
            override fun onConnectionStateChange(g: BluetoothGatt, status: Int, newState: Int) {
                if (newState == BluetoothProfile.STATE_CONNECTED) g.discoverServices()
                else if (!okConn.isCompleted) okConn.complete(false)
            }
            override fun onServicesDiscovered(g: BluetoothGatt, status: Int) {
                g.requestMtu(247)
                if (!okConn.isCompleted) okConn.complete(status == BluetoothGatt.GATT_SUCCESS)
            }
            override fun onCharacteristicChanged(g: BluetoothGatt, c: BluetoothGattCharacteristic, v: ByteArray) {
                if (!rspDeferred.isCompleted) rspDeferred.complete(v)
            }

            @Deprecated("Deprecated in Java")
            override fun onCharacteristicChanged(gatt: BluetoothGatt, characteristic: BluetoothGattCharacteristic) {
                val value = characteristic.value ?: return
                if (!rspDeferred.isCompleted) rspDeferred.complete(value)
            }
        })
        gattRef.set(gatt)
        okConn.await()
    }

    @SuppressLint("MissingPermission")
    suspend fun request(cmd: Byte, payload: ByteArray = byteArrayOf(), reqId: Byte = 1): ByteArray {
        val g = gattRef.get() ?: return byteArrayOf()
        val svc = g.getService(BleUuids.SERVICE_CONTROL) ?: return byteArrayOf()
        val ch = svc.getCharacteristic(BleUuids.CHAR_CMD_RX) ?: return byteArrayOf()

        rspDeferred = CompletableDeferred()
        val frame = BleProtocol.frame(cmd, reqId, payload)
        ch.value = frame
        g.writeCharacteristic(ch)
        return rspDeferred.await()
    }
}
