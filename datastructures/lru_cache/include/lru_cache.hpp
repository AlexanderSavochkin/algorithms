#pragma once

#include <utility>
#include <unordered_map>
#include <list>
#include <optional>

using std::pair;
using std::unordered_map;
using std::list;
using std::optional;
using std::make_pair;

template<typename TKey, typename TValue>
class lru_cache {
    int capacity;
    unordered_map<TKey, pair<TValue, typename list<TKey>::iterator>> data_directory;
    list<TKey> recency_ordered_keys;
public:
    lru_cache(int capacity) : capacity(capacity) {
    }

    optional<TValue> get(TKey key) {
        auto iter = data_directory.find(key);
        if (iter == data_directory.end())
        {
            return {};
        }
        else
        {
            auto [val, key_pointer_in_queue] = iter->second;

            recency_ordered_keys.erase(key_pointer_in_queue);
            //data_directory.erase(key);

            recency_ordered_keys.push_front(key);

            //data_directory.insert(make_pair(key, make_pair(val, recency_ordered_keys.begin())));
            data_directory.at(key).second = recency_ordered_keys.begin();
            return { val };
        }
    }

    bool contains(TKey key) {
		return data_directory.find(key) != data_directory.end();
	}

    void put(TKey key, TValue value) {
        auto iter = data_directory.find(key);
        if (iter == data_directory.end()) {
            if (data_directory.size() >= capacity) {
                //Delete LRU
                TKey least_recently_used_key = *recency_ordered_keys.rbegin();
                recency_ordered_keys.pop_back();
                data_directory.erase(least_recently_used_key);
            }
            recency_ordered_keys.push_front(key);
            data_directory.insert(make_pair(key, make_pair(value, recency_ordered_keys.begin())));
        }
        else {
            auto [val, key_pointer_in_queue] = iter->second;

            recency_ordered_keys.erase(key_pointer_in_queue);
            data_directory.erase(key);

            recency_ordered_keys.push_front(key);
            data_directory.insert(make_pair(key, make_pair(value, recency_ordered_keys.begin())));
        }
    }

    size_t size() {
		return data_directory.size();
	}
};
