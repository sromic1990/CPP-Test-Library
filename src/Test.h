#ifndef SOURAVTDD_TEST_H
#define SOURAVTDD_TEST_H
#include <ostream>
#include <string_view>
#include <vector>

namespace SouravTDD
{
    class MissingException
    {
        public:
            MissingException(std::string_view exType) : mExType(exType) {}
            std::string_view getExType() const { return mExType; }

        private:
            std::string_view mExType;
    };

    class TestBase
    {
        public:
            TestBase(std::string_view name) : mName(name), mPassed(true) {}
            virtual ~TestBase() = default;
            virtual void run() = 0;
            virtual void runEx() { run (); }
            std::string_view getName() const { return mName; }
            bool passed() const { return mPassed; }
            std::string_view getReason() const { return mReason; }
            std::string_view getExpectedReason() const { return mExpectedReason; }
            void setFailed(std::string_view reason) 
            { 
                mPassed = false; 
                mReason = reason; 
            }
            void setExpectedFailureReason(std::string_view reason) { mExpectedReason = reason; }

        private:
            std::string_view mName;
            bool mPassed;
            std::string_view mReason;
            std::string_view mExpectedReason;
    };

    inline std::vector<TestBase*>& getTests()
    {
        static std::vector<TestBase*> tests;
        return tests;
    }

    inline int runTests(std::ostream& output)
    {
        output      << "Running "
                    << getTests().size()
                    << " tests\n";

        int numPassed = 0;
        int numFailed = 0;
        int numMissedFailed = 0;

        for (auto * test : getTests())
        {
            output      <<"---------------\n"
                        << test->getName()
                        << std::endl;
            
            try
            {
                test->runEx();   
            }
            catch(MissingException const & ex)
            {
                std::string message = "Expected exception type ";
                message += ex.getExType();
                message += " was not thrown.";
                test->setFailed(message);
            }
            catch(...)
            {
                test->setFailed("Unexpected exception thrown.");
            }

            if (test->passed())
            {
                if(!test->getExpectedReason().empty())
                {
                    ++numMissedFailed;
                    output << "Missed expected failure\n"
                        << "Test passed but was expected to fail."
                        << std::endl;
                }
                else
                {
                    ++numPassed;
                    output << "PASSED\n";
                }
            }
            else if (!test->getExpectedReason().empty() && test->getReason() == test->getExpectedReason())
            {
                ++numPassed;
                output  << "Expected failure\n"
                        << test->getExpectedReason()
                        << std::endl;
            }
            
            else
            {
                ++numFailed;
                output << "FAILED: " << test->getReason() << "\n";
            }
        }

        output << "---------------\n";
        output << "Tests passed: " << numPassed << "\nTests failed: " << numFailed << std::endl;
        if (numMissedFailed != 0)
        {
            output << "Test failures missed: " << numMissedFailed << std::endl;
        }
        output << std::endl;
        return numFailed;
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

#define TEST_EX(testname, exceptionType) \
class SOURAVTDD_CLASS : public SouravTDD::TestBase \
{\
    public: \
        SOURAVTDD_CLASS (std::string_view name) : TestBase(name) \
            {\
                SouravTDD::getTests().push_back(this);\
            }\
        void runEx() override\
        {\
            try\
            {\
                run();\
            }\
            catch(exceptionType const &) \
            {\
                return;\
            }\
            throw SouravTDD::MissingException(#exceptionType);\
        }\
        void run() override;\
}; \
SOURAVTDD_CLASS SOURAVTDD_INSTANCE (testname); \
void SOURAVTDD_CLASS::run()

#endif //SOURAVTDD_TEST_H