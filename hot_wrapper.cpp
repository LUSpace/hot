#include "hot_wrapper.h"

extern "C" void *create_tree() {
  return reinterpret_cast<void *>(new hot_wrapper<uint64_t, char *>());
}