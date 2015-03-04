#ifndef REQUESTGENERATOR_H_INCLUDED
#define REQUESTGENERATOR_H_INCLUDED

#include "MemRequest.h"

class RequestGenerator
{
public:

    virtual ~RequestGenerator() {}

    virtual MemRequest nextRequest() = 0;
};

#endif // REQUESTGENERATOR_H_INCLUDED
