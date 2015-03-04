#include "CountedMemory.h"

using namespace std;

CountedMemory::CountedMemory( AbstractMemory & mem )
: mainMemory( mem ), referenceCount( 0 )
{}

CountedMemory::value_t CountedMemory::get( address_t address ) throw( range_error )
{
    throwIfOutOfRange( address );
    ++referenceCount;
    return mainMemory.get( address );
}

void CountedMemory::set( address_t address, value_t value ) throw( range_error )
{
    throwIfOutOfRange( address );
    ++referenceCount;
    mainMemory.set( address, value );
}

CountedMemory::address_t CountedMemory::size() const
{
    return mainMemory.size();
}

CountedMemory::count_t CountedMemory::getCount() const
{
    return referenceCount;
}

void CountedMemory::resetCount()
{
    referenceCount = 0;
}
