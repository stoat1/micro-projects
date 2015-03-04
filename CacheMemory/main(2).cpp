#include <iostream>
#include "Logger.h"
#include "CachedMemoryTest.h"
#include "CachedMemoryMeasuring.h"
#include "ManualTesting.h"

using namespace std;

int main()
{
    try
    {
//        cachedMemoryTest();
        startMeasuring();
//        startManualTesting();
    }
    catch( exception & ex )
    {
        cout << "Exception is occured:\n" << ex.what() << endl;
    }

    return 0;
}
