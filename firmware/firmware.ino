#include "Prelude.hpp"
#include "StorageManager.hpp"
#include "DeviceManager.hpp"
#include "APManager.hpp"
#include <Arduino_LSM6DS3.h>

// Libraries used:
// - https://github.com/khoih-prog/WiFiWebServer (MIT license)
// - https://github.com/arduino-libraries/SD (GPL-3.0 license)
// See https://support.arduino.cc/hc/en-us/articles/4415094490770-Licensing-for-products-based-on-Arduino

static StorageManager g_storageManager;
static DeviceManager  g_deviceManager;
static APManager      g_apManager;

// See comment in "Prelude.hpp"
IMUSample     g_imuSampleBuf[FW_PRES_IMU_CAP];
size_t        g_imuSampleIdx = 0;
unsigned long g_imuLastSample;

void setup() {
  // Wait for serial connection when in a debug build
#if FW_WANT_LOG
  Serial.begin(9600);
  while (!Serial);
  FW_LOG_PRINT("\n========================================");
  FW_LOG_PRINT("Mainboard Firmware Rev " FW_TO_STR(FW_REV));
#endif

  // Setup all components
  if (!g_storageManager.setup()) {
    Utility::panic("Unable to setup storage manager");
  }
  if (!g_deviceManager.setup()) {
    Utility::panic("Unable to setup device manager");
  }
  if (!g_apManager.setup()) {
    Utility::panic("Unable to setup AP manager");
  }

  // PRESENTATION: Initialize IMU driver
  if (!IMU.begin()) {
    Utility::panic("Unable to initialize IMU driver");
  }
  g_imuLastSample = millis();

  FW_LOG_PRINT("Initialized.");
}

void loop() {
  auto currentTime = millis();
  if (currentTime - g_imuLastSample > 2000) {
    auto &sample = g_imuSampleBuf[g_imuSampleIdx++];
    if (g_imuSampleIdx >= FW_PRES_IMU_CAP) {
      // good for now
      g_imuSampleIdx = 0;
    }
    IMU.readAcceleration(sample.accelX, sample.accelY, sample.accelZ);
    IMU.readGyroscope(sample.gyroX, sample.gyroY, sample.gyroZ);
    g_imuLastSample = currentTime;
  }

  g_apManager.poll();
}
