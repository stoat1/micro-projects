#ifndef ABSTRACT_ARRAY_INCLUDED
#define ABSTRACT_ARRAY_INCLUDED

#include <cstddef>
#include <string>
#include <ostream>
#include <stdexcept>

// abstract class: array of char with fixed size
class AbstractMemory
{
public:

    typedef size_t address_t;
    typedef int value_t;

    // constructor and destructor
    AbstractMemory();
    virtual ~AbstractMemory();

    // read and write
    virtual value_t get( address_t index ) throw( std::range_error ) = 0;
    virtual void set( address_t index, value_t data )
        throw( std::range_error ) = 0;

    // elems in array
    virtual address_t size() const = 0;

    // convert to string
    virtual std::string toString();

protected:

    void throwIfOutOfRange( address_t index ) throw( std::range_error );
};

std::ostream & operator<<( std::ostream & ost, AbstractMemory & mem );

#endif // ABSTRACT_ARRAY_INCLUDED
