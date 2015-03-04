#ifndef MACROREQUEST_H_INCLUDED
#define MACROREQUEST_H_INCLUDED

#include "Request.h"
#include <deque>

class MacroRequest : public Request
{
public:
    MacroRequest();
    ~MacroRequest();
    virtual void execute( AbstractMemory & memory );

    void addRequest( Request * request );

private:
    std::deque<Request *> reqs;
};

#endif // MACROREQUEST_H_INCLUDED
