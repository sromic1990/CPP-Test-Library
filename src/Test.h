/*
  _________                                 _________ .__            __    __                 __               
 /   _____/ ____  __ ______________ ___  __ \_   ___ \|  |__ _____ _/  |__/  |_  ___________ |__| ____   ____  
 \_____  \ /  _ \|  |  \_  __ \__  \\  \/ / /    \  \/|  |  \\__  \\   __\   __\/ __ \_  __ \|  |/ __ \_/ __ \ 
 /        (  <_> )  |  /|  | \// __ \\   /  \     \___|   Y  \/ __ \|  |  |  | \  ___/|  | \/|  \  ___/\  ___/ 
/_______  /\____/|____/ |__|  (____  /\_/    \______  /___|  (____  /__|  |__|  \___  >__/\__|  |\___  >\___  >
        \/                         \/               \/     \/     \/                \/   \______|    \/     \/ 

 * Copyright (c) 2024 Sourav Chatterjee
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef SOURAVTDD_TEST_H
#define SOURAVTDD_TEST_H
#include <ostream>
#include <string_view>
#include <vector>
#include <string>
#include <source_location>
#include <map>

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

    class Test;
    class TestSuite;

    inline std::map<std::string, std::vector<Test*>>&getTests()
    {
        static std::map<std::string, std::vector<Test*>> tests;
        return tests;
    }

    inline std::map<std::string, std::vector<TestSuite*>>& getTestSuites()
    {
        static std::map<std::string, std::vector<TestSuite*>> suites;
        return suites;
    }

    inline void addTests(std::string_view suiteName, Test* test)
    {
        std::string name(suiteName);
        if(!getTests().contains(name))
        {
            getTests().try_emplace(name, std::vector<Test*>());
        }
        getTests()[name].push_back(test);
    }

    inline void addTestSuits(std::string_view suiteName, TestSuite* suite)
    {
        std::string name(suiteName);
        if(!getTestSuites().contains(name))
        {
            getTestSuites().try_emplace(name, std::vector<TestSuite*>());
        }
        getTestSuites()[name].push_back(suite);
    }
    
    class TestBase
    {
        public:
            TestBase(std::string_view name, std::string_view suiteName) : 
            mName(name), 
            mSuiteName(suiteName), 
            mPassed(true), 
            mConfirmLocation(-1) 
            {}
            virtual ~TestBase() = default;
            std::string_view getName() const { return mName; }
            std::string_view getSuiteName() const { return mSuiteName; }
            bool passed() const { return mPassed; }
            std::string getReason() const { return mReason; }
            int getConfirmLocation() const { return mConfirmLocation; }
            void setFailed(std::string reason, int confirmLocation = -1) 
            { 
                mPassed = false; 
                mReason = reason;
                mConfirmLocation = confirmLocation;
            }

        private:
            std::string_view mName;
            std::string_view mSuiteName;
            bool mPassed;
            std::string mReason;
            int mConfirmLocation;
    };

    class TestSuite : public TestBase
    {
        public:
            TestSuite(std::string_view name, std::string_view suiteName) : TestBase(name, suiteName) 
            {
                addTestSuits(suiteName, this);
            }
            virtual void suiteSetup() = 0;
            virtual void suiteTeardown() = 0;
    };

    class Test : public TestBase
    {
        public:
            Test(std::string_view name, std::string_view suiteName = "") : TestBase(name, suiteName) 
            {
                addTests(suiteName, this);
            }
            virtual void run() = 0;
            virtual void runEx() { run (); }
            std::string getExpectedReason() const { return mExpectedReason; }
            void setExpectedFailureReason(std::string reason) { mExpectedReason = reason; }

        private:
            std::string mExpectedReason;
    };

    template <typename ExceptionT>
    class TestEx : public Test
    {
        public:
            TestEx(std::string_view name, std::string_view suiteName, std::string_view exceptionType)
                : Test(name, suiteName), mExceptionName(exceptionType) {}
            void runEx() override
            {
                try
                {
                    run();
                }
                catch(ExceptionT const &)
                {
                    return;
                }
                throw MissingException(mExceptionName);
            }

        private:
            std::string_view mExceptionName;
    };

    inline void runTest(std::ostream& output, Test* test, int& numPassed, int& numFailed, int& numMissedFailed)
    {
        output      << "-------Test: "
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

        if(test->passed())
        {
            if (!test->getExpectedReason().empty())
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
        else if(!test->getExpectedReason().empty() && test->getReason() == test->getExpectedReason())
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

    inline bool runSuite(std::ostream& output, bool setup, std::string const & name, int& numpassed, int& numfailed)
    {
        for (auto& suite: getTestSuites()[name])
        {
            if (setup)
            {
                output << "------- Setup";
            }
            else
            {
                output << "------- Teardown";
            }

            output  << suite->getName()
                    << std::endl;
            
            try
            {
                if(setup)
                {
                    suite->suiteSetup();
                }
                else
                {
                    suite->suiteTeardown();
                }
            }
            catch(ConfirmException const & ex)
            {
                suite->setFailed(ex.getReason(), ex.getLine());
            }
            catch(...)
            {
                suite->setFailed("Unexpected exception thrown.");
            }

            if(suite->passed())
            {
                ++numpassed;
                output << "PASSED\n";
            }
            else
            {
                ++numfailed;
                if(suite->getConfirmLocation() != -1)
                {
                    output << "FAILED: Confirm failed on line " << suite->getConfirmLocation() << "\n";
                }
                else
                output << "FAILED: \n" << suite->getReason() << "\n";
                return false;
            }
        }
        return true;
    }

    inline int runTests(std::ostream& output)
    {
        output      << "Running "
                    << getTests().size()
                    << " tests\n";

        int numPassed = 0;
        int numFailed = 0;
        int numMissedFailed = 0;

        for(auto const & [key, value] : getTests())
        {
            std::string suiteDisplayName = "Suite: ";
            if(key.empty())
            {
                suiteDisplayName += "Single Tests";
            }
            else
            {
                suiteDisplayName += key;
            }

            output  << "---------------"
                    << suiteDisplayName
                    << std::endl;

            if(!key.empty())
            {
                if(!getTestSuites().contains(key))
                {
                    output  << "Test suite not found."
                            << " Exiting test application."
                            << std::endl;

                    return ++numFailed;
                }

                if(!runSuite(output, true, key, numPassed, numFailed))
                {
                    output  << "Test suite setup failed."
                            << " Skipping tests in suite."
                            << std::endl;
                    continue;
                }
            }

            for(auto * test : value)
            {
                runTest(output, test, numPassed, numFailed, numMissedFailed);
            }

            if(!key.empty())
            {
                if(!runSuite(output, false, key, numPassed, numFailed))
                {
                    output  << "Test suite teardown failed."
                            << std::endl;
                }
            }
        }

        output  << "----------------------------------\n";
        output  << "Tests passed: " << numPassed
                << "\nTests failed: " << numFailed;
        
        if(numMissedFailed > 0)
        {
            output  << "\nTests failures missed: "
                    << numMissedFailed;
        }
        output << std::endl;
        return numFailed;
    }

    inline void confirm(bool expected, bool actual, const std::source_location location = std::source_location::current())
    {
        if (expected != actual)
        {
            throw BoolConfirmException(expected, location.line());
        }
    }

    inline void confirm(std::string_view expected, std::string_view actual, const std::source_location location = std::source_location::current())
    {
        if (expected != actual)
        {
            throw ActualConfirmException(expected, actual, location.line());
        }
    }

    inline void confirm(std::string expected, std::string actual, const std::source_location location = std::source_location::current())
    {
        if (expected != actual)
        {
            confirm(std::string_view(expected), std::string_view(actual));
        }
    }

    inline void confirm(float expected, float actual, const std::source_location location = std::source_location::current())
    {
        if (actual < (expected - 0.0001f) || actual > (expected + 0.0001f))
        {
            throw ActualConfirmException(std::to_string(expected), std::to_string(actual), location.line());
        }
    }

    inline void confirm(double expected, double actual, const std::source_location location = std::source_location::current())
    {
        if (actual < (expected - 0.000001) || actual > (expected + 0.000001))
        {
            throw ActualConfirmException(std::to_string(expected), std::to_string(actual), location.line());
        }
    }

    inline void confirm(long double expected, long double actual, const std::source_location location = std::source_location::current())
    {
        if (actual < (expected - 0.000001) || actual > (expected + 0.000001))
        {
            throw ActualConfirmException(std::to_string(expected), std::to_string(actual), location.line());
        }
    }

    template <typename T>
    void confirm(T const & expected, T const & actual, const std::source_location location = std::source_location::current())
    {
        if (expected != actual)
        {
            throw ActualConfirmException(std::to_string(expected), std::to_string(actual), location.line());
        }
    }

    template <typename T>
    class SetupAndTeardown : public T
    {
        public:
            SetupAndTeardown() { T::setup(); }
            ~SetupAndTeardown() { T::tearDown(); }
    };

    template <typename T>
    class TestSuiteSetupAndTearDown : public T, public TestSuite
    {
        public:
            TestSuiteSetupAndTearDown(std::string_view name, std::string_view suiteName) : TestSuite(name, suiteName) {}
            void suiteSetup() override { T::setup(); }
            void suiteTeardown() override { T::tearDown(); }  
    };
}

#define SOURAVTDD_CLASS_FINAL( line ) Test ## line
#define SOURAVTDD_CLASS_RELAY( line ) SOURAVTDD_CLASS_FINAL( line )
#define SOURAVTDD_CLASS SOURAVTDD_CLASS_RELAY( __LINE__ )
#define SOURAVTDD_INSTANCE_FINAL( line ) test ## line
#define SOURAVTDD_INSTANCE_RELAY( line ) SOURAVTDD_INSTANCE_FINAL( line )
#define SOURAVTDD_INSTANCE SOURAVTDD_INSTANCE_RELAY( __LINE__ )
#define TEST(testName) \
namespace\
{\
    class SOURAVTDD_CLASS : public SouravTDD::Test \
    {\
        public: \
            SOURAVTDD_CLASS (std::string_view name) : Test(name) \
            {}\
            void run() override;\
    }; \
}\
SOURAVTDD_CLASS SOURAVTDD_INSTANCE (testName); \
void SOURAVTDD_CLASS::run()

#define TEST_EX(testName, exceptionType) \
namespace\
{\
    class SOURAVTDD_CLASS : public SouravTDD::TestEx<exceptionType> \
    {\
        public: \
            SOURAVTDD_CLASS (std::string_view name, std::string_view exceptionName) : TestEx(name, "", exceptionName) \
            {}\
            void run() override;\
    }; \
}\
SOURAVTDD_CLASS SOURAVTDD_INSTANCE (testName, #exceptionType); \
void SOURAVTDD_CLASS::run()

#define TEST_SUITE(testName, suiteName) \
namespace\
{\
    class SOURAVTDD_CLASS : public SouravTDD::Test \
    {\
        public: \
            SOURAVTDD_CLASS (std::string_view name, std::string_view suite) : Test(name, suiteName) \
            {}\
            void run() override;\
    }; \
}\
SOURAVTDD_CLASS SOURAVTDD_INSTANCE (testName, suiteName); \
void SOURAVTDD_CLASS::run()

#define TEST_SUITE_EX(testName, suiteName, exceptionType) \
namespace\
{\
    class SOURAVTDD_CLASS : public SouravTDD::TestEx<exceptionType> \
    {\
        public: \
            SOURAVTDD_CLASS (std::string_view name, std::string_view suite, std::string_view exceptionName) : TestEx(name, suiteName, exceptionName) \
            {}\
            void run() override;\
    }; \
}\
SOURAVTDD_CLASS SOURAVTDD_INSTANCE (testName, suiteName, #exceptionType); \
void SOURAVTDD_CLASS::run()

#define CONFIRM_FALSE( actual )\
SouravTDD::confirm(false, actual)
#define CONFIRM_TRUE( actual )\
SouravTDD::confirm(true, actual)
#define CONFIRM(expected, actual)\
SouravTDD::confirm(expected, actual)
#endif //SOURAVTDD_TEST_H