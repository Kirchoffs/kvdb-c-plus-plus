#include <iostream>
#include <vector>
#include <memory>
#include "index/index.h"
#include "unit_test_framework.h"

DeclareTest(StdMapIndex, BasicOperations)

DefineTest(StdMapIndex, BasicOperations) {
    kvdb::StdMapIndex index;

    // Test put and get
    std::vector<uint8_t> key1 = {1, 2, 3};
    auto pos1 = std::make_shared<kvdb::LogRecordPos>(1, 42, 8);
    ExpectNull(index.put(key1, pos1));
     
    auto retrieved_pos1 = index.get(key1);
    ExpectEQ(retrieved_pos1->file_id, pos1->file_id);
    ExpectEQ(retrieved_pos1->offset, pos1->offset);

    // Test put empty key
    std::vector<uint8_t> empty_key;
    auto pos2 = std::make_shared<kvdb::LogRecordPos>(2, 43, 8);
    ExpectNull(index.put(empty_key, pos2));
    auto retrieved_pos2 = index.get(empty_key);
    ExpectEQ(retrieved_pos2->file_id, pos2->file_id);

    // Test get with non-existent key
    std::vector<uint8_t> non_existent_key = {4, 5, 6};
    ExpectNull(index.get(non_existent_key));

    // Test del
    ExpectNotNULL(index.del(key1));
    ExpectNull(index.get(key1));

    // Test del with non-existent key
    ExpectNull(index.del(non_existent_key));
}

int main() {
    RegisterTest(StdMapIndex, BasicOperations);
    RunTests();
}
