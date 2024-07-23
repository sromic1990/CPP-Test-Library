#include "Test.h"

bool isNegative(int number)
{
    return number < 0;
}

long long multiplyBy2(long long number)
{
    return number * 2LL;
}

TEST("Test will pass without any confirms")
{
}

TEST("Test bool confirms")
{
    bool result = isNegative(0);
    CONFIRM_FALSE(result);
    result = isNegative(-1);
    CONFIRM_TRUE(result);
}

TEST("Test int confims")
{
    int result = multiplyBy2(0);
    CONFIRM(0, result);
    result = multiplyBy2(1);
    CONFIRM(2, result);
    result = multiplyBy2(-1);
    CONFIRM(-2, result);
}

TEST("Test bool confirm failure")
{
    std::string reason = "Expected: true";
    setExpectedFailureReason(reason);
    bool result = isNegative(0);
    CONFIRM_TRUE(result);
}

TEST("Test int confirm failure")
{
    std::string reason = "Expected: 0\n";
    reason += "Actual: 2";
    setExpectedFailureReason(reason);
    int result = multiplyBy2(1);
    CONFIRM(0, result);
}

TEST("Test long confirms")
{
    long result = multiplyBy2(0L);
    CONFIRM(0L, result);
    result = multiplyBy2(1L);
    CONFIRM(2L, result);
    result = multiplyBy2(-1L);
    CONFIRM(-2L, result);
}

TEST("Test long confirm failure")
{
    std::string reason = "Expected: 0\n";
    reason += "Actual: 2";
    setExpectedFailureReason(reason);
    long result = multiplyBy2(1L);
    CONFIRM(0L, result);
}

TEST("Test long long confirms")
{
    long long result = multiplyBy2(0LL);
    CONFIRM(0LL, result);
    result = multiplyBy2(10'000'000'000LL);
    CONFIRM(20'000'000'000LL, result);
    result = multiplyBy2(-10'000'000'000LL);
    CONFIRM(-20'000'000'000LL, result);
}

TEST("Test long long confirm failure")
{
    std::string reason = "Expected: 10000000000\n";
    reason += "Actual: 20000000000";
    setExpectedFailureReason(reason);
    long long result = multiplyBy2(10'000'000'000LL);
    CONFIRM(10'000'000'000LL, result);
}

TEST("Test string confirms")
{
    std::string result = "abc";
    std::string expected = "abc";
    CONFIRM(expected, result);
}

TEST("Test string confirm failure")
{
    std::string reason = "Expected: def\n";
    reason += "Actual: abc";
    setExpectedFailureReason(reason);
    std::string result = "abc";
    std::string expected = "def";
    CONFIRM(expected, result);
}