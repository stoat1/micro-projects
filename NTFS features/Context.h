#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <windows.h>
#include <map>
#include <vector>

#include "Config.h"
#include "ContextObserver.h"

using namespace std;

class Context
{
public:

    struct Localization
    {
    public:

        Localization()
        : language(""), content()
        {}

        Localization(const string & lang, const Config & conf)
        : language(lang), content(conf)
        {}

        string language;
        Config content;
    };

    Context();

    void reset();
    void loadFromFile(const string & fileName) throw(runtime_error);
    void saveToFile(const string & fileName) throw(runtime_error);

    map<string, Localization> & getContent();

    // returns localization of specified language
    Config * getLocal(const string & streamName);

    void addObserver(ContextObserver * observer);

private:

    void notifyObservers();

    // map stream name to Localization
    map<string, Localization> contents;

    vector<ContextObserver *> observers;
};

#endif // CONTEXT_H_INCLUDED
