#include "MacroRequest.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

using namespace std;

MacroRequest::MacroRequest()
{}


class Deleter : unary_function<Request *, void>
{
public:

    void operator()( Request * p )
    {
        delete p;
    }
};

MacroRequest::~MacroRequest()
{
    for_each( reqs.begin(), reqs.end(), Deleter() );
}

class Executer
{
public:

    Executer( AbstractMemory & memory )
    : memory( memory ) {}

    void operator()( Request * request )
    {
        request->execute( memory );
    }

private:

    AbstractMemory & memory;
};

void MacroRequest::execute( AbstractMemory & memory )
{
    for_each( reqs.begin(), reqs.end(), Executer( memory ) );
}

void MacroRequest::addRequest( Request * request )
{
    reqs.push_back( request );
}
