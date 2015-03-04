#include <iostream>

#include "Context.h"
#include "SerialGenerator.h"

using namespace std;

void Context::reset()
{
    const Config & defaultLocal = Config::getDefaultLocalization();

    contents[""] = Localization("<default>", defaultLocal);
    contents["en"] = Localization("English", defaultLocal);
    contents["ru"] = Localization("Russian", defaultLocal);
    contents["eo"] = Localization("Esperanto", defaultLocal);

    notifyObservers();
}

void Context::loadFromFile(const string & fileName) throw(runtime_error)
{
    for(map<string, Localization>::iterator iItem = contents.begin();
        iItem != contents.end(); ++iItem)
    {
        Config & content = iItem->second.content;
        const string & streamName = iItem->first;

        content.loadFromFileStream(fileName, streamName);
    }

    notifyObservers();
}

void Context::saveToFile(const string & fileName) throw(runtime_error)
{
    for(map<string, Localization>::iterator iItem = contents.begin();
        iItem != contents.end(); ++iItem)
    {
        Config & content = iItem->second.content;
        const string & streamName = iItem->first;

        content.saveToFileStream(fileName, streamName);
    }
}

void Context::addObserver(ContextObserver * observer)
{
    observers.push_back(observer);
}

Context::Context()
: contents()
, observers()
{
    reset();
}

Config * Context::getLocal(const string & streamName)
{
    map<string, Localization>::iterator iFound;

    iFound = contents.find(streamName);
    if(contents.end() == iFound)
    {
        return NULL;
    }
    else
    {
        return & iFound->second.content;
    }
}

map<string, Context::Localization> & Context::getContent()
{
    return contents;
}

void Context::notifyObservers()
{
    for(size_t i = 0; i < observers.size(); ++i)
    {
        observers.at(i)->contentChanged();
    }
}
