#ifndef LOG_RECORD_H
#define LOG_RECORD_H

#include <cstdint>
#include <vector>

namespace kvdb {

class LogRecordPos {
public:
    uint32_t file_id;
    int64_t offset;
    uint32_t size;
    LogRecordPos() = default;
    LogRecordPos(uint32_t file_id, int64_t offset, uint32_t size): file_id(file_id), offset(offset), size(size) {}
};

enum LogRecordType {
    NORMAL,
    DELETE,
    TX_FINISHED
};

class LogRecord {
public:
    std::vector<uint8_t> key;
    std::vector<uint8_t> value;
    LogRecordType type;
};

};

#endif
