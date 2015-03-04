#ifndef SERIALGENERATOR_H_INCLUDED
#define SERIALGENERATOR_H_INCLUDED

#include <windows.h>

class SerialGenerator
{
public:

    HMENU operator()()
    {
        ++x;
        return reinterpret_cast<HMENU>(x);
    }

    static SerialGenerator & getInstance()
    {
        static SerialGenerator instance;
        return instance;
    }

private:

    SerialGenerator()
    : x(1000)
    {}

    int x;
};

#endif // SERIALGENERATOR_H_INCLUDED
