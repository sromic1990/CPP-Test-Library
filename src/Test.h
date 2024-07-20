#ifndef SOURAVTDD_TEST_H
#define SOURAVTDD_TEST_H

#include <string_view>
#include <vector>

namespace SouravTDD
{
    class TestInterface
    {
        public:
            virtual ~TestInterface() = default;
            virtual void run() = 0;
    };

    inline std::vector<TestInterface*>& getTests()
    {
        static std::vector<TestInterface*> tests;
        return tests;
    }

    inline void runTests()
    {
        for (auto * test : getTests())
        {
            test->run();
        }
    }
}

#define TEST \
class Test : public SouravTDD::TestInterface \
{\
    public: \
        Test(std::string_view name) : mName(name), mResult(true) \
            {\
                SouravTDD::getTests().push_back(this);\
            }\
        void run() override;\
    private: \
        std::string_view mName; \
        bool mResult; \
}; \
Test test ("testCanBeCreated"); \
void Test::run()

#endif //SOURAVTDD_TEST_H