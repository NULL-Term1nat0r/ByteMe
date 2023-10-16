#pragma once

#include <Arduino.h>

// Build configuration
#define FW_REV      1002
#define FW_WANT_LOG 1
#define FW_SSID     "ByteMe"
#define FW_PSK      "datalogger1"

// Identification signature; do not touch, please :)
#define FW_MAGIC    "ByteMe\xffAudi\x00"

// Optional logging
#if FW_WANT_LOG
# define FW_LOG_WRITE(...) Serial.print(__VA_ARGS__)
# define FW_LOG_PRINT(...) Serial.println(__VA_ARGS__)
#else
# define FW_LOG_WRITE(...)
# define FW_LOG_PRINT(...)
#endif // FW_WANT_LOG

// Utility macros
#define __packed __attribute__((packed))
#define FW_TO_STR_INNER(x) #x
#define FW_TO_STR(x) FW_TO_STR_INNER(x)

namespace Utility {
  [[noreturn]]
  void panic(const char *reason);

  static inline void wait_for_interrupt() {
    asm volatile ("wfi");
  }
};

// PRESENTATION: IMU sampling to SRAM; reasons:
//               - Flash won't work with RP2040 Connect
//               - SD card not available
struct IMUSample {
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;
};
#define FW_PRES_IMU_CAP 8000
extern IMUSample     g_imuSampleBuf[FW_PRES_IMU_CAP];
extern size_t        g_imuSampleIdx;
extern unsigned long g_imuLastSample;
