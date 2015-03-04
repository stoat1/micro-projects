#include "CachedMemoryTest.h"
#include "CachedMemory.h"
#include "PhysicalMemory.h"
#include "CountedMemory.h"
#include "Random.h"
#include "Logger/Logger.h"

using namespace std;

int test( const size_t MAIN_MEMORY_SIZE, const size_t CACHE_SIZE, const size_t GROUP_WIDTH )
{
    PhysicalMemory physicalMem( MAIN_MEMORY_SIZE );
    CountedMemory physCountMem( physicalMem );
    CachedMemory cachedMem( physCountMem, CACHE_SIZE, GROUP_WIDTH );
    CountedMemory cachCountMem( cachedMem );

    AbstractMemory & mem = cachCountMem;

    Random random( 0 );
    for( size_t i = 0; i < mem.size(); ++i )
    {
        char r = random.nextValue() % 128;
        mem.set( i, r );
    }

    random.reset( 0 );
    for( size_t i = 0; i < mem.size(); ++i )
    {
        char r = random.nextValue() % 128;
        if( mem.get( i ) != r )
        {
            return i;
        }
    }

    random.reset( 0 );
    for( size_t i = 0; i < mem.size(); ++i )
    {
        char r = random.nextValue() % 128;
        mem.set( i, r );
    }

    random.reset( 0 );
    for( size_t i = 0; i < mem.size(); ++i )
    {
        char r = random.nextValue() % 128;
        if( mem.get( i ) != r )
        {
            return i;
        }
    }

    float probability = 1 - static_cast<float>( physCountMem.getCount() )
                        / cachCountMem.getCount();
    return 0;
}

void cachedMemoryTest()
{
    test( 10, 1, 0 );
    test( 10, 10, 1 );
}
