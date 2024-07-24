#ifndef SOURAVTDD_TEST_H
#define SOURAVTDD_TEST_H
#include <ostream>
#include <string_view>
#include <vector>
#include <string>

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

    class ConfirmException
    {
        public:
            ConfirmException(int line) : mLine(line) {}
            ConfirmException() = default;
            virtual ~ConfirmException() = default;
            std::string getReason() const { return mReason; }
            int getLine() const { return mLine; }

        protected:
            std::string mReason;
            int mLine;
    };

    class BoolConfirmException : public ConfirmException
    {
        public:
            BoolConfirmException(bool expected, int line) : ConfirmException(line)
            {
                mReason = "Expected: ";
                mReason += expected ? "true" : "false";
            }
    };

    class ActualConfirmException : public ConfirmException
    {
        public:
            ActualConfirmException(std::string_view expected, std::string_view actual, int line) : mExpected(expected), mActual(actual), ConfirmException(line)
            {
                formatReason();
            }

        private:
            void formatReason()
            {
                mReason = "Expected: ";
                mReason += mExpected;
                mReason += "\nActual: ";
                mReason += mActual;
            }

            std::string_view mExpected;
            std::string_view mActual;
    };

    class TestBase
    {
        public:
            TestBase(std::string_view name) : mName(name), mPassed(true), mConfirmLocation(-1) {}
            virtual ~TestBase() = default;
            virtual void run() = 0;
            virtual void runEx() { run (); }
            std::string_view getName() const { return mName; }
            bool passed() const { return mPassed; }
            std::string getReason() const { return mReason; }
            std::string getExpectedReason() const { return mExpectedReason; }
            int getConfirmLocation() const { return mConfirmLocation; }
            void setFailed(std::string reason, int confirmLocation = -1) 
            { 
                mPassed = false; 
                mReason = reason;
                mConfirmLocation = confirmLocation;
            }
            void setExpectedFailureReason(std::string reason) 
            { 
                mExpectedReason = reason;
            }

        private:
            std::string_view mName;
            bool mPassed;
            std::string mReason;
            std::string mExpectedReason;
            int mConfirmLocation;
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
            catch(ConfirmException const & ex)
            {
                test->setFailed(ex.getReason(), ex.getLine());
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

            // output << "Expected: " << test->getExpectedReason() << std::endl;
            // output << "Actual: " << test->getReason() << std::endl;

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
                if(test->getConfirmLocation() != -1)
                {
                    output << "FAILED: Confirm failed on line " << test->getConfirmLocation() << "\n";
                }
                else
                    output << "FAILED: \n" << test->getReason() << "\n";

                output << test->getReason() << std::endl;
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

    inline void confirm(bool expected, bool actual, int line)
    {
        if (expected != actual)
        {
            throw BoolConfirmException(expected, line);
        }
    }

    inline void confirm(std::string_view expected, std::string_view actual, int line)
    {
        if (expected != actual)
        {
            throw ActualConfirmException(expected, actual, line);
        }
    }

    inline void confirm(std::string expected, std::string actual, int line)
    {
        if (expected != actual)
        {
            confirm(std::string_view(expected), std::string_view(actual), line);
        }
    }

    inline void confirm(float expected, float actual, int line)
    {
        if (actual < (expected - 0.0001f) || actual > (expected + 0.0001f))
        {
            throw ActualConfirmException(std::to_string(expected), std::to_string(actual), line);
        }
    }

    inline void confirm(double expected, double actual, int line)
    {
        if (actual < (expected - 0.000001) || actual > (expected + 0.000001))
        {
            throw ActualConfirmException(std::to_string(expected), std::to_string(actual), line);
        }
    }

    inline void confirm(long double expected, long double actual, int line)
    {
        if (actual < (expected - 0.000001) || actual > (expected + 0.000001))
        {
            throw ActualConfirmException(std::to_string(expected), std::to_string(actual), line);
        }
    }

    template <typename T>
    void confirm(T const & expected, T const & actual, int line)
    {
        if (expected != actual)
        {
            throw ActualConfirmException(std::to_string(expected), std::to_string(actual), line);
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
namespace\
{\
    class SOURAVTDD_CLASS : public SouravTDD::TestBase \
    {\
        public: \
            SOURAVTDD_CLASS (std::string_view name) : TestBase(name) \
            {\
                SouravTDD::getTests().push_back(this);\
            }\
            void run() override;\
    }; \
}\
SOURAVTDD_CLASS SOURAVTDD_INSTANCE (testname); \
void SOURAVTDD_CLASS::run()

#define TEST_EX(testname, exceptionType) \
namespace\
{\
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
}\
SOURAVTDD_CLASS SOURAVTDD_INSTANCE (testname); \
void SOURAVTDD_CLASS::run()

#define CONFIRM_FALSE( actual )\
SouravTDD::confirm(false, actual, __LINE__)
#define CONFIRM_TRUE( actual )\
SouravTDD::confirm(true, actual, __LINE__)
#define CONFIRM(expected, actual)\
SouravTDD::confirm(expected, actual, __LINE__)
#endif //SOURAVTDD_TEST_H