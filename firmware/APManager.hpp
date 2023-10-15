#pragma once

#include "Prelude.hpp"

#include <WiFiWebServer.hpp>

class APManager {
public:
  APManager();

  bool setup();
  void poll();

private:
  WiFiWebServer m_server;
};
