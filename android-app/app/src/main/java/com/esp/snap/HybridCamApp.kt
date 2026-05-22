package com.esp.snap

import android.app.Application

class HybridCamApp : Application() {
    lateinit var db: AppDatabase
        private set

    override fun onCreate() {
        super.onCreate()
        db = AppDatabase.build(this)
    }
}
