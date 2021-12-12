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
        std::pair<const char *, P> *,
        idx::contenthelpers::PairPointerKeyExtractor>();
  }

  void bulk_load(const V bulk_arr[], int num) {
    char path[1024];
    for (int i = 0; i < 10; ++i) {
      memcpy(path, bulk_arr[i].first->key, bulk_arr[i].first->length);
      path[bulk_arr[i].first->length] = '\0';
      printf("%s\n", path);
    }

    for (int i = 0; i < num; ++i) {
      char fixed_string_key[256] = {0};
      memcpy(fixed_string_key, bulk_arr[i].first->key,
             bulk_arr[i].first->length);
      fixed_string_key[bulk_arr[i].first->length] = '\0';
      auto val =
          new std::pair<const char *, P>(fixed_string_key, bulk_arr[i].second);
      my_tree->insert(val);
    }
  }

  bool insert(const T &key, const P &payload) {
    if constexpr (std::is_pointer_v<T>) {
      char fixed_string_key[256] = {0};
      memcpy(fixed_string_key, key->key, key->length);
      fixed_string_key[key->length] = '\0';
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
      char path[1024];
      memcpy(path, key->key, key->length);
      path[key->length] = '\0';
      printf("%s\n", path);

      char fixed_string_key[256] = {0};
      memcpy(fixed_string_key, key->key, key->length);
      fixed_string_key[key->length] = '\0';
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