#pragma once

#include <iostream>
#include <list>
#include <unordered_map>

struct cache_stats {
  long hits;
  long misses;
  long evictions;
};

template <typename Key, typename Value> class lru_cache {
  using KeyValue = typename std::pair<Key, Value>;
  using ListIter = typename std::list<KeyValue>::iterator;

private:
  std::unordered_map<Key, ListIter> map_;
  std::list<KeyValue> items_list_;
  size_t capacity_;
  cache_stats stats_{};

public:
  explicit lru_cache(size_t capacity) : capacity_(capacity), map_(capacity) {}

  void insert(const Key& key, const Value& value) {
    std::cout << "Inserting: key={" << key << "}, value={" << value << "}\n";
    auto search = map_.find(key);
    if (search != map_.end()) {
      std::cout << "Found " << search->first << " "
                << "\n";
      ListIter iter = search->second;
      iter->second = value;
      move_lru(iter);
    } else {
      ListIter elem = items_list_.insert(items_list_.begin(), {key, value});
      map_.insert({key, elem});
      check_list_size();
    }
    print_list();
    std::cout << std::endl;
  }

  std::optional<Value> get(const Key& key) {
    auto search = map_.find(key);
    if (search == map_.end()) {
      stats_.misses += 1;
      return {};
    } else {
      stats_.hits += 1;
      ListIter& it = search->second;
      move_lru(it);
      Value& v = it->second;
      return std::optional<Value>{v};
    }
  }

  void remove(const Key& key) {
    auto search = map_.find(key);
    if (search != map_.end()) {
      items_list_.erase(search->second);
      map_.erase(search);
    }
  }

  [[nodiscard]] size_t count() const noexcept { return map_.size(); }

  bool contains(const Key& key) const { return map_.find(key) != map_.end(); }

  [[nodiscard]] const cache_stats& stats() const { return stats_; }

private:
  void check_list_size() {
    if (items_list_.size() > capacity_) {
      KeyValue last = items_list_.back();
      map_.erase(last.first);
      items_list_.pop_back();
      stats_.evictions += 1;
    }
  }

  inline void move_lru(const ListIter& it) {
    if (it != items_list_.begin()) {
      items_list_.splice(items_list_.begin(), items_list_, it);
    }
  }

  void print_list() {
    std::cout << "List: ";
    for (auto& [k, v] : items_list_) {
      std::cout << k << "=" << v << ", ";
    }
    std::cout << "\n";
  }
};
