#ifndef INDEX_H
#define INDEX_H

#include <vector>
#include <memory>
#include <map>
#include <shared_mutex>
#include "data/log_record.h"

namespace kvdb {

class Index {
public:
    virtual ~Index() = default;
    virtual std::shared_ptr<LogRecordPos> put(const std::vector<uint8_t>& key, std::shared_ptr<LogRecordPos> pos) = 0;
    virtual std::shared_ptr<LogRecordPos> get(const std::vector<uint8_t>& key) = 0;
    virtual std::shared_ptr<LogRecordPos> del(const std::vector<uint8_t>& key) = 0;
};

class StdMapIndex : public Index {
public:
    StdMapIndex() = default;
    std::shared_ptr<LogRecordPos> put(const std::vector<uint8_t>& key, std::shared_ptr<LogRecordPos> pos) override;
    std::shared_ptr<LogRecordPos> get(const std::vector<uint8_t>& key) override;
    std::shared_ptr<LogRecordPos> del(const std::vector<uint8_t>& key) override;

private:
    std::map<std::vector<uint8_t>, std::shared_ptr<LogRecordPos>> index;
    mutable std::shared_mutex mtx;
};

};

#endif
