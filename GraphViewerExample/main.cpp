// main.cpp
// example of using Graphs.exe

// Build Options... -> Linker Settings -> Add, then input path to libGraph.a

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include "Graph.h"

using namespace std;

void createFermatSpiral(Graph & g, double step = 1)
{
    // set name
    ostringstream osstr;
    osstr << "Fermat's spiral with step = " << step << endl;
    g.setName(osstr.str());

    // add points
    g.clear();
    for(double phi = 0; phi < 4 * M_PI; phi += step)
    {
        double r1 = sqrt( phi ), r2 = - r1;

        g.push_back(r1 * cos(phi), r1 * sin(phi));
        g.push_back(r2 * cos(phi), r2 * sin(phi));
    }
}

void saveGraph(string filename, const Graph & g)
{
    ofstream ofstr(filename.c_str());
    ofstr << g;
}

int main()
{
    Graph g;

    createFermatSpiral(g, 1);
    saveGraph("FermatSpiral1.txt", g);

    createFermatSpiral(g, 0.5);
    saveGraph("FermatSpiral2.txt", g);

    createFermatSpiral(g, 0.25);
    saveGraph("FermatSpiral3.txt", g);

    system("start ..\\GraphViewer\\bin\\GraphViewer.exe\
            FermatSpiral1.txt FermatSpiral2.txt FermatSpiral3.txt");

    return 0;
}
