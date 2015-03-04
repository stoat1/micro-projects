#include "CachedMemoryMeasuring.h"
#include "PhysicalMemory.h"
#include "CachedMemory.h"
#include "CountedMemory.h"
#include "CategoricalRequests.h"
#include "ParetoRequests.h"
#include "Graph.h"
#include <fstream>

using namespace std;

double getCacheHit(
    const size_t mainMemorySize,
    const size_t cacheSize,
    const size_t cacheGroupWidth,
    const size_t flagResetFrequency,

    RequestGenerator * generator
)
{
    const size_t REQUESTS = 10 * 1000;

    AbstractMemory * mainMemory = new PhysicalMemory( mainMemorySize );
    CountedMemory * countedMainMemory = new CountedMemory( * mainMemory );
    CachedMemory * cachedMemory = new CachedMemory(
        * countedMainMemory,
        cacheSize,
        cacheGroupWidth,
        flagResetFrequency );
    CountedMemory * countedCachedMemory = new CountedMemory( * cachedMemory );

    while( countedCachedMemory->getCount() < REQUESTS )
    {
        MemRequest req = generator->nextRequest();
        req.execute( * countedCachedMemory );
    }

    size_t requestCount = countedCachedMemory->getCount();
    size_t missCount = countedMainMemory->getCount();

    double hitProbability = 1 - static_cast<double>( missCount ) / requestCount;

    delete countedCachedMemory;
    delete cachedMemory;
    delete countedMainMemory;
    delete mainMemory;

    return hitProbability;
}

#define PRINT_PARAMS()\
    cout << "mem size: \t" << mainMemSize << endl;\
    cout << "cache size: \t" << cacheSize << endl;\
    cout << "flagResetFreq: \t" << flagResetFreq << endl;\
    cout << "cache group width: \t" << cacheGroupWidth << endl;\
    cout << "deque size: \t" << dequeSize << endl;\
    cout << "minX: \t" << minGroupSize << " alpha: \t" << alpha << endl;\
    cout << ">> hit = " << hit << endl;\
    cout << endl;

void startMeasuring()
{
    vector<string> filenames;
    string filename;
    ofstream ofst;
    Graph * g;
    RequestGenerator * gen;
    size_t count;

    const size_t mainMemSize = 1024;
    const size_t cacheGroupWidth = 2;
    const size_t flagResetFreq = 40;
    const size_t cacheSize = mainMemSize / 8;

    const size_t dequeSize = 3;
    const size_t minGroupSize = 1;
    const double alpha = 2;

    filename = "hit=f(cacheSize).txt";
    filenames.push_back( filename );
    g = new Graph("x: cacheSize, y: cache hit");
    gen = new ParetoRequests( mainMemSize, dequeSize, minGroupSize, alpha );
    for( size_t cacheSize = 1 << cacheGroupWidth; cacheSize < mainMemSize / 4; cacheSize += cacheSize / 5 + 1 )
    {
        double hit = getCacheHit(
            mainMemSize,
            cacheSize,
            cacheGroupWidth,
            flagResetFreq,
            gen
        );
        PRINT_PARAMS();
        Point point( cacheSize, hit );
        g->push_back( point );
    }
    delete gen;

    ofst.open( filename.c_str(), ios::out );
    ofst << * g;
    ofst.close();
    delete g;

    filename = "hit=f(cacheGroupWidth).txt";
    filenames.push_back( filename );
    g = new Graph("x: cacheGroupWidth, y: cache hit");
    gen = new ParetoRequests( mainMemSize, dequeSize, minGroupSize, alpha );
    for( size_t cacheGroupWidth = 0; 1 << cacheGroupWidth <= cacheSize; cacheGroupWidth += 1 )
    {
        double hit = getCacheHit(
            mainMemSize,
            cacheSize,
            cacheGroupWidth,
            flagResetFreq,
            gen
        );
        PRINT_PARAMS();
        Point point( cacheGroupWidth, hit );
        g->push_back( point );
    }
    delete gen;

    ofst.open( filename.c_str(), ios::out );
    ofst << * g;
    ofst.close();
    delete g;

    filename = "hit=f(flagResetFreq).txt";
    filenames.push_back( filename );
    g = new Graph("x: flags reset frequency, y: cache hit");
    gen = new ParetoRequests( mainMemSize, dequeSize, minGroupSize, alpha );
    for( size_t flagResetFreq = 1; flagResetFreq < 1000; flagResetFreq += flagResetFreq / 5 + 1 )
    {
        double hit = getCacheHit(
            mainMemSize,
            cacheSize,
            cacheGroupWidth,
            flagResetFreq,
            gen
        );
        PRINT_PARAMS();
        Point point( flagResetFreq, hit );
        g->push_back( point );
    }
    delete gen;

    ofst.open( filename.c_str() );
    ofst << * g;
    ofst.close();
    delete g;

    filename = "hit=f(minGroupSize).txt";
    filenames.push_back( filename );
    g = new Graph("x: pareto minX, y: cache hit");
    for( size_t minGroupSize = 1; minGroupSize < 20; minGroupSize += 1 )
    {
        gen = new ParetoRequests( mainMemSize, dequeSize, minGroupSize, alpha );
        double hit = getCacheHit(
            mainMemSize,
            cacheSize,
            cacheGroupWidth,
            flagResetFreq,
            gen
        );
        PRINT_PARAMS();
        delete gen;
        Point point( minGroupSize, hit );
        g->push_back( point );
    }

    ofst.open( filename.c_str() );
    ofst << * g;
    ofst.close();
    delete g;

    filename = "hit=f(alpha).txt";
    filenames.push_back( filename );
    g = new Graph("x: pareto alpha, y: cache hit");
    for( double alpha = 0.1; alpha < 3; alpha += 0.05 )
    {
        gen = new ParetoRequests( mainMemSize, dequeSize, minGroupSize, alpha );
        double hit = getCacheHit(
            mainMemSize,
            cacheSize,
            cacheGroupWidth,
            flagResetFreq,
            gen
        );
        delete gen;
        PRINT_PARAMS();
        Point point( alpha, hit );
        g->push_back( point );
    }

    ofst.open( filename.c_str() );
    ofst << * g;
    ofst.close();
    delete g;

    filename = "hit=f(dequeSize).txt";
    filenames.push_back( filename );
    g = new Graph("x: dequeSize, y: cache hit");
    for( size_t dequeSize = 1; dequeSize < 5; dequeSize += 1 )
    {
        gen = new ParetoRequests( mainMemSize, dequeSize, minGroupSize, alpha );
        double hit = getCacheHit(
            mainMemSize,
            cacheSize,
            cacheGroupWidth,
            flagResetFreq,
            gen
        );
        delete gen;
        PRINT_PARAMS();
        Point point( dequeSize, hit );
        g->push_back( point );
    }

    ofst.open( filename.c_str() );
    ofst << * g;
    ofst.close();
    delete g;

    filename = "address=f(t).txt";
    filenames.push_back( filename );
    g = new Graph("x: dequeSize, y: cache hit");
    gen = new ParetoRequests( mainMemSize, dequeSize, minGroupSize, alpha );
    count = 0;
    while( count < 1000 )
    {
        vector<MemRequest::address_t> reqs;
        gen->nextRequest().toVector( reqs );
        for( size_t i = 0; i < reqs.size(); ++i )
        {
            ++count;
            Point point( count, reqs.at( i ) );
            g->push_back( point );
        }
    }
    delete gen;

    ofst.open( filename.c_str() );
    ofst << * g;
    ofst.close();
    delete g;

    filename = "filesize=f().txt";
    filenames.push_back( filename );
    g = new Graph("x: filesize, y: nothing");
    Distribution * distr = new ParetoDistribution( minGroupSize, alpha );
    double fcount = 0;
    while( fcount < 1000 )
    {
        double par = distr->nextValue();
        fcount += par;
        Point point( fcount, par );
        g->push_back( point );
    }
    delete distr;

    ofst.open( filename.c_str() );
    ofst << * g;
    ofst.close();
    delete g;

    string args = "";
    for( vector<string>::iterator iter = filenames.begin();
        iter != filenames.end(); ++iter )
    {
        args += " " + * iter;
    }
    system( ( "start ..\\GraphViewer\\bin\\GraphViewer.exe " + args ).c_str() );
}
