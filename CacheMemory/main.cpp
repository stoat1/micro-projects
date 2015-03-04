#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cassert>

#include "StreamMemory.h"
#include "PhysicalMemory.h"
#include "CachedMemory.h"
#include "CountedMemory.h"
#include "NormalDistribution.h"
#include "Random.h"
#include "CachedMemoryTest.h"

// TODO put cache class to namespace

using namespace std;

int main()
{
    try
    {
        cachedMemoryTest();
    }
    catch( exception & ex )
    {
        cerr << "Exception: " << ex.what() << endl;
    }
}
