#include "Prelude.hpp"
#include "APManager.hpp"
#include "StaticResources.hpp"

#include <WiFiWebServer.h>

APManager::APManager(): m_server(80) {
}

bool APManager::setup() {
  // Check if a WiFi module is present
  if (WiFi.status() == WL_NO_MODULE) {
    FW_LOG_PRINT("APManager::setup(): WiFi.status() returned WL_NO_MODULE");
    return false;
  }

  // Create an access point using the configured FW_SSID and FW_PSK
  int status = WiFi.beginAP(FW_SSID, FW_PSK);
  if (status != WL_AP_LISTENING) {
    FW_LOG_PRINT("APManager::setup(): Unable to begin AP");
    return false;
  }

  // Add routes for static resources
  for (size_t index = 0; g_staticResources[index] != nullptr; index++) {
    auto resource = g_staticResources[index];
    m_server.on(resource->path, [this, resource]()
    {
      m_server.send(200, resource->mime, (const char *)resource->data, resource->size);
    });
  }

  // Start the server
  m_server.begin();
  return true;
}

void APManager::poll() {
  m_server.handleClient();
}
