#ifndef UNIT_TEST_FRAMEWORK_H
#define UNIT_TEST_FRAMEWORK_H

#include <iostream>
#include <vector>

#define DeclareTest(Module, TestName)                             \
class Test_##Module##_##TestName: public UnitTest {               \
private:                                                          \
    Test_##Module##_##TestName(): UnitTest() {}                   \
                                                                  \
public:                                                           \
    static Test_##Module##_##TestName* get_instance() {           \
        static Test_##Module##_##TestName testClass;              \
        return &testClass;                                        \
    }                                                             \
    void run_func() override;                                     \
};

#define DefineTest(Module, TestName)                              \
void Test_##Module##_##TestName::run_func()

#define RegisterTest(Module, TestName)                            \
    UnitTest::get_instance().test_list.push_back(                 \
        Test_##Module##_##TestName::get_instance()                \
    );

#define RunTests()                                                \
    UnitTest::get_instance().run_tests();

#define ExpectEQ(arg1, arg2)                                      \
    isTrue &= expect_eq(arg1, arg2);

#define ExpectNull(arg)                                           \
    isTrue &= expect_null(arg);

#define ExpectNotNULL(arg)                                        \
    isTrue &= !expect_null(arg);

class UnitTest {
protected:
    UnitTest() {}

public:
    bool isTrue{true};
    static std::vector<UnitTest*> test_list;
    
    virtual ~UnitTest() {}
    virtual void run_func() {}

    static UnitTest& get_instance() {
        static UnitTest unitTest;
        return unitTest;
    }
    
    void run_tests();

    template<typename T>
    inline bool expect_eq(const T& arg1, const T& arg2) {
        bool isTrue{arg1 == arg2};
        return isTrue;
    }

    template<typename T>
    inline bool expect_null(const std::shared_ptr<T>& arg) {
        bool isTrue{arg == nullptr};
        if (!isTrue) {
            std::cout << "Actual: " << arg << " vs. "
                      << "Expected: nullptr" << std::endl;
        }
        return isTrue;
    }
};

#endif
