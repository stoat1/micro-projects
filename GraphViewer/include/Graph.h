#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <ostream>
#include <istream>
#include <vector>

struct Point
{
public:
    Point( double x, double y )
    : x( x ), y( y )
    {}

    double x;
    double y;
};

class Graph : public std::vector<Point>
{
public:

    Graph(std::string name = "untitled");

    void setName(std::string name);
    std::string getName() const;

    void push_back(Point p);
    void push_back(double x, double y);

    friend std::ostream & operator<<( std::ostream & ost, const Graph & g );
    friend std::istream & operator>>( std::istream & ist, Graph & g );

private:

    std::string name;
};

std::ostream & operator<<( std::ostream & ost, const Graph & g );
std::istream & operator>>( std::istream & ist, Graph & g );

#endif // GRAPH_H_INCLUDED
