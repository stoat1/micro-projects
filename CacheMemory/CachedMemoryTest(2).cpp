#include "CachedMemoryTest.h"
#include "CachedMemory.h"
#include "PhysicalMemory.h"
#include "CountedMemory.h"
#include "Random.h"
#include "Logger.h"
#include "NormalDistribution.h"
#include "UniformRequests.h"

using namespace std;

int test(
    const size_t MAIN_MEMORY_SIZE,
    const size_t CACHE_SIZE,
    const size_t GROUP_WIDTH
)
{
    Logger & log = Logger::getInstance( "tests" );
    ostringstream osstr;

    osstr << "In test(MAIN_MEMORY_SIZE=" << MAIN_MEMORY_SIZE
        << ", CACHE_SIZE=" << CACHE_SIZE
        << ", GROUP_WIDTH=" << GROUP_WIDTH << ")";
    log.println( osstr );

    PhysicalMemory physicalMem( MAIN_MEMORY_SIZE );
    CountedMemory physCountMem( physicalMem );
    CachedMemory cachedMem( physCountMem, CACHE_SIZE, GROUP_WIDTH, GROUP_WIDTH * 3 );
    CountedMemory cachCountMem( cachedMem );

    PhysicalMemory mirror( MAIN_MEMORY_SIZE ),
        mirror2( MAIN_MEMORY_SIZE );
    AbstractMemory & mem = cachCountMem;

    RequestGenerator * gener = new UniformRequests( MAIN_MEMORY_SIZE );

    for( int i = 0; i < 10; ++i )
    {
        MemRequest req = gener->nextRequest();

        req.execute( mem );
        req.execute( mirror );
        req.execute( mirror2 );
    }

    for( size_t i = 0; i < MAIN_MEMORY_SIZE; ++i )
    {
        if( mirror.get( i ) != mirror2.get( i ) )
        {
            osstr << "fix line " << __LINE__ << endl;
            log.println( osstr );
        }
        if( mirror.get( i ) != mem.get( i ) )
        {
            osstr << "fail: mirror[i] != mem[i] at i = " << i << endl;
            osstr << "mem=" << mem << endl << "mirror=" << mirror << endl
                << "mirror2=" << mirror2;
            log.println( osstr );
            break;
        }
    }

    delete gener;

    return 0;
}

void cachedMemoryTest()
{
    Logger & log = Logger::getInstance( "Tests" );
    log.println( "In cachedMemoryTest()" );

    test( 2, 1, 0 );
    test( 10, 1, 0 );
    test( 10, 10, 1 );
    test( 10, 5, 1 );
    test( 1000, 100, 2 );
}
