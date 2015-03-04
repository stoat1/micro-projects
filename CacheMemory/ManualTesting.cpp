#include <string>
#include <iostream>
#include <sstream>

#include "PhysicalMemory.h"
#include "CachedMemory.h"
#include "ManualTesting.h"

using namespace std;

typedef AbstractMemory::value_t value_t;
typedef AbstractMemory::address_t address_t;

void startManualTesting()
{
    const size_t MAIN_MEM_SIZE = 10;
    const size_t CACHE_SIZE = 4;
    const size_t GROUP_WIDTH = 0;
    const size_t FLAG_RESET_FREQ = 6;

    PhysicalMemory physicalMem( MAIN_MEM_SIZE );
    CachedMemory cachedMem(
        physicalMem,
        CACHE_SIZE,
        GROUP_WIDTH,
        FLAG_RESET_FREQ );

    string line;
    string command;

    while( true )
    {
        cout << "(manual test) ";

        getline( cin, line );
        istringstream isstr( line );
        isstr >> command;

        if( "w" == command )
        {
            value_t val;
            address_t addr;

            isstr >> addr;
            isstr >> val;

            cachedMem.set( addr, val );
        }
        else if( "r" == command )
        {
            value_t val;
            address_t addr;

            isstr >> addr;

            val = cachedMem.get( addr );

            cout << val << endl;
        }
        else if( "p" == command )
        {
            cout << cachedMem << endl;
        }
        else if( "q" == command )
        {
            break;
        }
    }
}
