#include <iostream>
#include "Logger.h"

using namespace std;

int main()
{
    Logger & log = Logger::getInstance( "WinAPI debug" );
    ostringstream ost;

    log.println( "Test logging" );

    ost << "pi \\approx " << 3.14 << " and e \\approx " << 2.1828;
    log.println( ost.str() );
    ost.str("");

    return 0;
}
