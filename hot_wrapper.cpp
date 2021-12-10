#include "hot_wrapper.hpp"

extern "C" void *create_tree() {
  return reinterpret_cast<void *>(new hot_wrapper<string_key *, char *>());
}