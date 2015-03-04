#ifndef INTERVAL_H_INCLUDED
#define INTERVAL_H_INCLUDED

#include "AbstractMemory.h"

struct Interval
{
    typedef AbstractMemory::address_t address_t;

    address_t begin;
    address_t end;
};

#endif // INTERVAL_H_INCLUDED
