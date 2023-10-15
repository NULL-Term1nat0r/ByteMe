#include "Prelude.hpp"
#include "StorageManager.hpp"
#include "DeviceManager.hpp"
#include "APManager.hpp"

// Libraries used:
// - https://github.com/khoih-prog/WiFiWebServer (MIT license)
// - https://github.com/arduino-libraries/SD (GPL-3.0 license)
// See https://support.arduino.cc/hc/en-us/articles/4415094490770-Licensing-for-products-based-on-Arduino

static StorageManager g_storageManager;
static DeviceManager  g_deviceManager;
static APManager      g_apManager;

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
}

void loop() {
  g_apManager.poll();
}
