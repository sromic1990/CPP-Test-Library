#include "Test.h"

bool isPassingGrade(int grade)
{
    return true;
}

TEST("Test will pass without any confirms")
{
}

TEST("Test passing grades")
{
    bool result = isPassingGrade(0);
    if(result)
    {
        throw 1;
    }

    result = isPassingGrade(100);
    if(!result)
    {
        throw 1;
    }
}