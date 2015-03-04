#ifndef DEFAULTLOCALIZATION_H_INCLUDED
#define DEFAULTLOCALIZATION_H_INCLUDED

#include <map>
#include <stdexcept>
#include <windows.h>
#include <ostream>

using namespace std;

class Config
{
public:

    Config();

    void loadFromFileStream(const string & fileName, const string & streamName)
        throw(runtime_error);

    void saveToFileStream(const string & fileName, const string & streamName)
        throw(runtime_error);

    static const Config & getDefaultLocalization();

    map<string, string> & getMap();

    const map<string, string> & getMap() const;

private:

    bool splitLine(const string & line, string & key, string & value);
    bool readString(HANDLE hFile, string & str);

    void addMissing();

    map<string, string> mapping;
};

ostream & operator<<(ostream & ost, const Config & config);

#endif // DEFAULTLOCALIZATION_H_INCLUDED
