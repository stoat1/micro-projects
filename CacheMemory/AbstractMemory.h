#ifndef ABSTRACT_ARRAY_INCLUDED
#define ABSTRACT_ARRAY_INCLUDED

#include <cstddef>
#include <string>

// abstract class: array of char with fixed size
class AbstractMemory
{
public:

    typedef size_t address_t;
    typedef char value_t;

    // constructor and destructor
    AbstractMemory();
    virtual ~AbstractMemory();

    // read and write
    virtual value_t get( address_t index ) = 0;
    virtual void set( address_t index, value_t data ) = 0;

    // elems in array
    virtual address_t size() const = 0;

    // convert to string
    virtual std::string toString();
};

#endif // ABSTRACT_ARRAY_INCLUDED
