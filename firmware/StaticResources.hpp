#pragma once

#include "Prelude.hpp"

struct StaticResource {
  const char    *path;
  const char    *mime;
  const uint8_t *data;
  size_t         size;
};

// == HOW TO (RE-)BUILD ==
// - Enter the repo's root directory
// - Execute the following command:
//   scripts/gen_frontend_pack.py > firmware/StaticResources.cpp
extern const StaticResource *g_staticResources[];
