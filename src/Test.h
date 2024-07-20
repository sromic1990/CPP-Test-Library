#ifndef TEST_H
#define TEST_H

#include <string_view>

#define TEST class Test \
{\
    public:\
        Test (std::string_view name) : mName(name), mResult(true) {}\
        void operator() () const;\
    private:\
        std::string_view mName;\
        bool mResult; \
};\
Test test("testCanBeCreated");\
void Test::operator() () const

#endif  // TEST_H