#include "index/index.h"
#include "data/log_record.h"

std::shared_ptr<kvdb::LogRecordPos> kvdb::StdMapIndex::put(const std::vector<uint8_t>& key, std::shared_ptr<kvdb::LogRecordPos> pos) {
    std::unique_lock<std::shared_mutex> lock(mtx);
    auto it = index.find(key);
    if (it != index.end()) {
        auto prev = it->second;
        it->second = pos;
        return prev;
    }
    index[key] = pos;
    return nullptr;
}

std::shared_ptr<kvdb::LogRecordPos> kvdb::StdMapIndex::get(const std::vector<uint8_t>& key) {
    std::shared_lock<std::shared_mutex> lock(mtx);
    auto it = index.find(key);
    if (it == index.end()) {
        return nullptr;
    }
    return it->second;
}

std::shared_ptr<kvdb::LogRecordPos> kvdb::StdMapIndex::del(const std::vector<uint8_t>& key) {
    std::unique_lock<std::shared_mutex> lock(mtx);
    auto it = index.find(key);
    if (it == index.end()) {
        return nullptr;
    }
    auto prev = it->second;
    index.erase(it);
    return prev;
}
