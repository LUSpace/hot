#pragma once
#include "tree.h"
#include <hot/rowex/HOTRowex.hpp>
#include <idx/contenthelpers/PairPointerKeyExtractor.hpp>
#include <map>
// used to define the interface of all benchmarking trees
template <class T, class P> class hot_wrapper : public Tree<T, P> {
public:
  typedef std::pair<T, P> V;

  hot_wrapper() {
    // create an instance of hot
    my_tree = new hot::rowex::HOTRowex<
        std::pair<char *, P> *, idx::contenthelpers::PairPointerKeyExtractor>();
  }

  void bulk_load(const V bulk_arr[], int num) {
    for (int i = 0; i < num; ++i) {
      char fixed_string_key[257];
      memcpy(fixed_string_key, bulk_arr[i].first->key,
             bulk_arr[i].first->length);
      auto val =
          new std::pair<const char *, P>(fixed_string_key, bulk_arr[i].second);
      my_tree->insert(val);
    }
  }

  bool insert(const T &key, const P &payload) {
    if constexpr (std::is_pointer_v<T>) {
      char fixed_string_key[257];
      memcpy(fixed_string_key, key->key, key->length);
      auto val = new std::pair<const char *, P>(fixed_string_key, payload);
      return my_tree->insert(val);
    } else {
      // LOG_FATAL("The key must be string key in HOT!");
      std::cout << "The key must be string key in HOT!" << std::endl;
      exit(-1);
    }
  }

  bool search(const T &key, P *payload) const {
    if constexpr (std::is_pointer_v<T>) {
      char fixed_string_key[257];
      memcpy(fixed_string_key, key->key, key->length);
      auto result = my_tree->lookup(fixed_string_key);
      if (result.mIsValid) {
        *payload = result.mValue->second;
        return true;
      } else {
        return false;
      }
    } else {
      // LOG_FATAL("The key must be string key in HOT!");
      std::cout << "The key must be string key in HOT!" << std::endl;
      exit(-1);
    }
  }

  // 0 means no erase, 1 means erase 1
  bool erase(const T &key) { return false; }

  bool update(const T &key, const P &payload) { return false; }

  void print_min_max() {}

  void get_depth_info() {}

  int range_scan_by_size(const T &key, uint32_t to_scan, V *&result = nullptr) {
    return 0;
  }

private:
  hot::rowex::HOTRowex<std::pair<const char *, P> *,
                       idx::contenthelpers::PairPointerKeyExtractor>
      *my_tree; // Store the pointer to the tree instance
};