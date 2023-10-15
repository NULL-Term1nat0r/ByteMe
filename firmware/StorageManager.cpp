#include "Prelude.hpp"
#include "StorageManager.hpp"

struct __packed Superblock {
  char     magic[sizeof(FW_MAGIC)];
  uint32_t fw_rev;
  uint64_t blk_count;
  char     reserved[488];
};

bool StorageManager::setup() {
  // TODO: Implement this
  return true;
}
