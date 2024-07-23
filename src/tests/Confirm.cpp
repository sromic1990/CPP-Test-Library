#include "Test.h"

bool isNegative(int number)
{
    return number < 0;
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