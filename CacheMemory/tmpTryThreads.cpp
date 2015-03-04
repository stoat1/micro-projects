#include <iostream>
#include "AssocSearch.h"
#include <vector>

using namespace std;

class Equals : public AssocSearch<vector<int>::iterator>::Predicate
{
public:
    Equals( int x ) : x( x ) {}
    bool operator()( const int & x )
    {
        return x == this->x;
    }
private:
    int x;
};

int main()
{
    vector<int> v( 10 );
    for( size_t i = 0; i < v.size(); ++i )
    {
        v[i] = i + 100;
    }

    AssocSearch<vector<int>::iterator> as( v.begin(), v.end(), 2 );

    for( int i = 0; i < v.size() + 1; ++i )
    {
        Equals eq( i + 100 );

        vector<int>::iterator found = as.findIf( & eq );
        if( found == v.end() )
        {
            cout << "not found" << endl;
        }
        else
        {
            cout << "found " << * found << endl;
        }
    }

    return 0;
}
