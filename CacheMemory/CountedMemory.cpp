#include "CountedMemory.h"

CountedMemory::CountedMemory( AbstractMemory & mem )
: mainMemory( mem ), referenceCount( 0 )
{}

CountedMemory::value_t CountedMemory::get( address_t address )
{
    ++referenceCount;
    return mainMemory.get( address );
}

void CountedMemory::set( address_t address, value_t value )
{
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
