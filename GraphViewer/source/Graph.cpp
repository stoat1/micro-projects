#include "../include/Graph.h"
#include "SDL_gfxPrimitives.h"
#include <sstream>

using namespace std;

Graph::Graph( string name )
: name( name )
{}

void Graph::setName(string name)
{
    this->name = name;
}

string Graph::getName() const
{
    return name;
}

void Graph::push_back(Point p)
{
    vector<Point>::push_back(p);
}

void Graph::push_back(double x, double y)
{
    vector<Point>::push_back(Point(x, y));
}

ostream & operator<<( ostream & ost, const Graph & g )
{
    // write name
    ost << g.name << endl;

    // write points
    for( vector<Point>::const_iterator iter = g.begin();
        iter != g.end(); ++iter )
    {
        ost << iter->x << " " << iter->y << endl;
    }

    return ost;
}

istream & operator>>( istream & ist, Graph & g )
{
    // read name
    getline( ist, g.name, '\n' );

    // read points
    g.clear();
    while( ist.good() )
    {
        double x, y;

        ist >> x >> y;
        if( ist.good() )
        {
            g.push_back( Point( x, y ) );
        }
    }

    return ist;
}
