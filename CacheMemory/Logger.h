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
    explicit Logger( const std::string & name, std::ostream * ost );

    Logger & operator=( const Logger & orig );

public:

    Logger( const Logger & orig );
    ~Logger();

    void println( const std::string & line );
    void println( std::ostringstream & line );

    static Logger & getInstance( const std::string & name );

    static void createFileLogger( const std::string & loggerName,
                             const std::string & fileName );

    static void createStreamLogger( const std::string & loggerName,
                             std::ostream * ostr );

private:

    static void init( std::map<std::string, Logger *> & loggers );

    static std::map<std::string, Logger *> loggers;

    std::string name;
    HANDLE mux;
    std::ostream * ost;
};

#endif // LOGGER_H_INCLUDED
