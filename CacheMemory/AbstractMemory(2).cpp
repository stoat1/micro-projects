#include "AbstractMemory.h"

#include <sstream>

using namespace std;

// constructor
AbstractMemory::AbstractMemory()
{}

// destructor
AbstractMemory::~AbstractMemory()
{}

// convert to string
string AbstractMemory::toString()
{
    ostringstream resultBuffer;

    for( address_t i = 0; i < size(); ++i )
    {
        value_t data = get( i );
        resultBuffer << data;
    }

    return resultBuffer.str();
}

ostream & operator<<( ostream & ost, AbstractMemory & mem )
{
    ost << "AbstractMemory[" << endl;

    ost << "\t" << "address : value" << endl;
    for( AbstractMemory::address_t addr = 0; addr < mem.size(); ++addr )
    {
        ost << '\t' << addr << " : " << mem.get( addr ) << endl;
    }

    ost << "]";
    return ost;
}

void AbstractMemory::throwIfOutOfRange( address_t index ) throw( range_error )
{
    if( index < size() )
    {
        return;
    }
    else
    {
        ostringstream osstr;
        osstr << "index = " << index << " is not in [0; " << size() <<")";
        throw range_error(osstr.str());
    }
}
