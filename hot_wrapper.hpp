#pragma once
#include "tree_api.h"
#include <hot/rowex/HOTRowex.hpp>
#include <idx/contenthelpers/PairPointerKeyExtractor.hpp>
#include <map>
// used to define the interface of all benchmarking trees
class hot_wrapper : tree_api {
public:
  hot_wrapper() {
    // create an instance of hot
    my_tree = new hot::rowex::HOTRowex<
        std::pair<const char *, char *> *,
        idx::contenthelpers::PairPointerKeyExtractor>();
  }

  bool insert(const char *key, size_t key_sz, const char *value,
              size_t value_sz) override {
    char *payload = const_cast<char *>(*reinterpret_cast<const char **>(value));
    char *fixed_string_key = new char[256]();
    memcpy(fixed_string_key, key, key_sz);
    fixed_string_key[key_sz] = '\0';
    auto val = new std::pair<const char *, char *>(fixed_string_key, payload);
    return my_tree->insert(val);
  }

  bool find(const char *key, size_t sz, char *value_out) override {
    char fixed_string_key[256] = {0};
    memcpy(fixed_string_key, key, sz);
    fixed_string_key[sz] = '\0';
    auto result = my_tree->lookup(fixed_string_key);
    if (result.mIsValid) {
      memcpy(value_out, &(result.mValue->second), sizeof(uint64_t));
      return true;
    } else {
      return false;
    }
  }

  bool update(const char *key, size_t key_sz, const char *value,
              size_t value_sz) override {
    return true;
  }

  bool remove(const char *key, size_t key_sz) override { return true; }

  int scan(const char *key, size_t key_sz, int scan_sz,
           char *&values_out) override {
    return 0;
  }

private:
  hot::rowex::HOTRowex<std::pair<const char *, char *> *,
                       idx::contenthelpers::PairPointerKeyExtractor>
      *my_tree; // Store the pointer to the tree instance
};