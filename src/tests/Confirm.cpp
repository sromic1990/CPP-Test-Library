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

TEST("Test bool pointer dereference confirms")
{
    bool result1 = true;
    bool result2 = false;
    bool* pResult1 = &result1;
    bool* pResult2 = &result2;
    CONFIRM_TRUE(*pResult1);
    CONFIRM_FALSE(*pResult2);
}

TEST("Test string and string literal confirms")
{
    std::string result = "abc";
    CONFIRM("abc", result);
}

TEST("Test float confirms")
{
    float f1 = 0.1f;
    float f2 = 0.2f;
    float sum = f1 + f2;
    float expected = 0.3f;
    CONFIRM(expected, sum);
}

TEST("Test double confirms")
{
    double d1 = 0.1;
    double d2 = 0.2;
    double sum = d1 + d2;
    double expected = 0.3;
    CONFIRM(expected, sum);
}

TEST("Test long double confirms")
{
    long double ld1 = 0.1L;
    long double ld2 = 0.2L;
    long double sum = ld1 + ld2;
    long double expected = 0.3L;
    CONFIRM(expected, sum);
}