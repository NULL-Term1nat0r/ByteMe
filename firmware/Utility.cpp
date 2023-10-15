#include "Prelude.hpp"

void Utility::panic(const char *cause) {
  FW_LOG_PRINT("\n** PANIC **");
  FW_LOG_WRITE("=> Cause: ");
  FW_LOG_PRINT(cause);

  // Wait for reset
  for (;;) {
    Utility::wait_for_interrupt();
  }
}
