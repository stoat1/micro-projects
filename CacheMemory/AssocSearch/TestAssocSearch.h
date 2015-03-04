#ifndef TESTASSOCSEARCH_H_INCLUDED
#define TESTASSOCSEARCH_H_INCLUDED

#include "AssocSearch.h"
#include <vector>
#include <algorithm>
#include <functional>
#include "../Logger/Logger.h"

using namespace std;

int nextInt()
{
    static int result = 100;
    return result++;
}

bool myPred( const vector<int>::iterator::value_type & i )
{
    return i == 110;
}

void testAssocSearch()
{
    Logger & log = Logger::getInstance( "tests" );
    ostringstream osstr;

    try
    {
        size_t N = 10;
        size_t M = 3;

        vector<int> vect( N + 1 );
        AssocSearch<vector<int>::iterator> assSearch( vect.begin(), vect.end() - 1, M );

        generate( vect.begin(), vect.end() - 1, nextInt );
        vect.back() = -1;

        assSearch.setCollection( vect.begin(), vect.end() - 1 );

        vector<int>::iterator result = assSearch.findIf( myPred );

        osstr << "Result is " << * result;
        log.println( osstr );
    }
    catch( exception & ex )
    {
        osstr << "Exception: " << ex.what();
        log.println( osstr );
    }
}

#endif // TESTASSOCSEARCH_H_INCLUDED
