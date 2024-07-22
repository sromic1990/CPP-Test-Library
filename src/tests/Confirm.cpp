#include "Test.h"

bool isPassingGrade(unsigned int grade)
{
    return grade >= 60;
}

TEST("Test will pass without any confirms")
{
}

TEST("Test passing grades")
{
    bool result = isPassingGrade(0);
    CONFIRM_FALSE(result);

    result = isPassingGrade(100);
    CONFIRM_TRUE(result);
}