package com.esp.snap

data class EspSettings(
    val jpegQuality: Int = 10,
    val resolution: Int = 10,
    val brightness: Int = 1,
    val contrast: Int = 1,
    val saturation: Int = 0,
    val sharpness: Int = 1,
    val flashMode: Int = 2,
    val flashIntensity: Int = 140,
    val chunkSize: Int = 180,
    val retryCount: Int = 5
)
