#include "unit_test_framework.h"

std::vector<UnitTest*> UnitTest::test_list;

void UnitTest::run_tests() {
    int passed{0};
    int failed{0};

    if (test_list.empty()) {
        std::cout << "No tests registered!" << std::endl;
        return;
    }

    for (UnitTest* x : test_list) {
        x->run_func();
        x->isTrue ? ++passed : ++failed;
    }
    
    std::cout << "Passed: " << passed << std::endl;
    std::cout << "Failed: " << failed << std::endl;

    if (failed > 0) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}
