#include "fio/fio_manager.h"

#include <fcntl.h>

kvdb::FileIOManager::FileIOManager(const std::string& filename) : filename(filename) {
    fd = open(filename.c_str(), O_RDWR | O_CREAT, 0644);
}

kvdb::Result<int> kvdb::FileIOManager::read(std::vector<uint8_t>& data, int64_t offset) {
    if (fd == -1) {
        return Result<int>::Err(ErrorType::FailedOpenDataFile);
    }
}
