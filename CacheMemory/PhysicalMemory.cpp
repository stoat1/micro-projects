#include "PhysicalMemory.h"

PhysicalMemory::PhysicalMemory( address_t s )
: vect( s )
{}

PhysicalMemory::~PhysicalMemory()
{}

PhysicalMemory::value_t PhysicalMemory::get( address_t i )
{
    return vect[ i ];
}

void PhysicalMemory::set( address_t i, value_t d )
{
    vect[ i ] = d;
}

PhysicalMemory::address_t PhysicalMemory::size() const
{
    return vect.size();
}
