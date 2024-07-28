#include "Test.h"
#include <string_view>

int createTestEntry()
{
    //If this was real code, it might open a
    //connection to a database, insert a row of data,
    //and return the row identifier.
    return 100;
}

void updateTestEntryName(int /*id*/, std::string_view name)
{
    if(name.empty())
    {
        //If this was real code, it might throw an
        //exception because the name is empty.
        throw 1;
    }
    //Real code would proceed to update the data with new name.
}

void deleteTestEntry(int /*id*/)
{
    //If this was real code, it might delete the row
    //from the database.
}

class TempEntry
{
    public:
       void setup()
       {
            mId = createTestEntry();
       }

       void tearDown()
       {
            deleteTestEntry(mId);
       }

       int getId()
       {
            return mId;
       }

    private:
        int mId;
};


TEST_EX("Test will run setup and teardown code", int)
{
    SouravTDD::SetupAndTeardown<TempEntry> entry;
    //If this was a project, it might be called
    //"Updating Empty name throws." And the type thrown
    //would be an int.
    updateTestEntryName(entry.getId(), "");
}