#include "Prelude.hpp"
#include "DeviceManager.hpp"

#include <Wire.h>

bool DeviceManager::setup() {
  Wire.begin();
  return true;
}

void DeviceManager::probe() {
  // TODO: Implement this
}
