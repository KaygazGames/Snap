package com.esp.snap

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.viewModels
import com.esp.snap.databinding.ActivityMainBinding

class MainActivity : ComponentActivity() {
    private lateinit var binding: ActivityMainBinding
    private val vm: MainViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        PermissionHelper.requestBlePermissions(this)

        binding.btnScanConnect.setOnClickListener { vm.scanAndConnect(this) }
        binding.btnSync.setOnClickListener { vm.manualSync(this) }
        binding.btnGetStatus.setOnClickListener { vm.refreshStatus() }

        vm.uiState.observe(this) { s ->
            binding.tvStatus.text = s
        }
    }
}
