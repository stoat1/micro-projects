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
