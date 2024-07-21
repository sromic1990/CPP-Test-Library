#ifndef SOURAVTDD_TEST_H
#define SOURAVTDD_TEST_H

#include <iostream>
#include <string_view>
#include <vector>

namespace SouravTDD
{
    class TestBase
    {
        public:
            TestBase(std::string_view name) : mName(name), mPassed(true) {}
            virtual ~TestBase() = default;
            virtual void run() = 0;
            std::string_view getName() const { return mName; }
            bool passed() const { return mPassed; }
            std::string_view getReason() const { return mReason; }
            void setFailed(std::string_view reason) 
            { 
                mPassed = false; 
                mReason = reason; 
            }

        private:
            std::string_view mName;
            bool mPassed;
            std::string_view mReason;
    };

    inline std::vector<TestBase*>& getTests()
    {
        static std::vector<TestBase*> tests;
        return tests;
    }

    inline void runTests()
    {
        for (auto * test : getTests())
        {
            std::cout<<"---------------\n"
            << test->getName()
            << std::endl;
            
            try
            {
                test->run();   
            }
            catch(...)
            {
                test->setFailed("Unexpected exception thrown");
            }

            if (test->passed())
            {
                std::cout << "PASSED\n";
            }
            else
            {
                std::cout << "FAILED: " << test->getReason() << "\n";
            }
        }
    }
}

#define SOURAVTDD_CLASS_FINAL( line ) Test ## line
#define SOURAVTDD_CLASS_RELAY( line ) SOURAVTDD_CLASS_FINAL( line )
#define SOURAVTDD_CLASS SOURAVTDD_CLASS_RELAY( __LINE__ )
#define SOURAVTDD_INSTANCE_FINAL( line ) test ## line
#define SOURAVTDD_INSTANCE_RELAY( line ) SOURAVTDD_INSTANCE_FINAL( line )
#define SOURAVTDD_INSTANCE SOURAVTDD_INSTANCE_RELAY( __LINE__ )
#define TEST(testname) \
class SOURAVTDD_CLASS : public SouravTDD::TestBase \
{\
    public: \
        SOURAVTDD_CLASS (std::string_view name) : TestBase(name) \
            {\
                SouravTDD::getTests().push_back(this);\
            }\
        void run() override;\
}; \
SOURAVTDD_CLASS SOURAVTDD_INSTANCE (testname); \
void SOURAVTDD_CLASS::run()

#endif //SOURAVTDD_TEST_H