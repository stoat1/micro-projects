#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <ostream>
#include <string>
#include <sstream>
#include <map>
#include <memory>
#include "windows.h"

class Logger
{
private:

    // create logger which write to file
    explicit Logger( std::ostream * ost );

    Logger( const Logger & orig );
    Logger & operator=( const Logger & orig );

public:

    ~Logger();

    void println( const std::string & line );
    void println( std::ostringstream & line );

    static Logger & getInstance( const std::string & name );

private:

    static void init( std::map<std::string, Logger *> & loggers );

    HANDLE mux;
    std::ostream * ost;
};

#endif // LOGGER_H_INCLUDED
