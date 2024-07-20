#include <iostream>
#include <string_view>

class Test
{
    public:
        Test (std::string_view name) : mName(name) {}
        void operator() () const
        {
            std::cout << mName << std::endl;
        
        }

    private:
        std::string_view mName;
        bool mResult; 
};

Test test("testCanBeCreated");

int main()
{
    std::cout << "Hello, World!" << std::endl;

    test();

    return 0;
}