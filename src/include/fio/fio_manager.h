#ifndef FILE_IO_H
#define FILE_IO_H

#include "utils/result.h"

#include <utility>
#include <vector>

namespace kvdb {

class IOManager {
public:
    virtual ~IOManager() = default;
    virtual Result<int> read(std::vector<uint8_t>& data, int64_t offset) = 0;
    virtual Result<int> write(const std::vector<uint8_t>& data) = 0;
    virtual bool sync() = 0;
    virtual bool close() = 0;
};

class FileIOManager : public IOManager {
public:
    FileIOManager(const std::string& filename);
    ~FileIOManager() override;
    Result<int> read(std::vector<uint8_t>& data, int64_t offset) override;
    Result<int> write(const std::vector<uint8_t>& data) override;
    bool sync() override;
    bool close() override;

private:
    int fd;
    std::string filename;
};

};

#endif
