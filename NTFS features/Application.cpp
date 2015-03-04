#include "Application.h"
#include <algorithm>
#include <iostream>
#include <windows.h>

Application::Application()
: filename("")
, changedStreams()
, observers()
, opened(false)
, saved(true)
{}

void Application::newFile() throw(runtime_error, logic_error)
{
    if(opened)
    {
        throw logic_error("calling newFile() while file is opened");
    }

    opened = true;
    saved = false;

    notifyObserversAboutStatus();
}

void Application::openFile(const string & filename) throw(runtime_error, logic_error)
{
    if(opened)
    {
        throw logic_error("calling newFile() while file is opened");
    }

    this->filename = filename;

    // TODO open file

    opened = true;
    saved = true;

    notifyObserversAboutFileName();
    notifyObserversAboutStatus();
}

void Application::saveFile(const string & filename) throw(runtime_error, logic_error)
{
    if(! opened)
    {
        throw logic_error("calling newFile() while file is opened");
    }

    this->filename = filename;

    // TODO save file

    saved = true;

    notifyObserversAboutFileName();
    notifyObserversAboutStatus();
}

void Application::closeFile() throw(runtime_error)
{
    if(! opened)
    {
        throw logic_error("calling newFile() while file is opened");
    }

    // TODO close file

    this->filename = "";

    opened = false;;
    saved = true;

    notifyObserversAboutFileName();
    notifyObserversAboutStatus();
}

bool Application::isFileOpened() const
{
    return opened;
}

bool Application::isFileSaved() const
{
    return saved;
}

void Application::setStreamContent(const string & streamName, const string & content) throw(runtime_error)
{
    cout << "Application::setStreamContent(" << streamName << ", " << content << ")" << endl;

    HANDLE hFile;
    DWORD dwRet;

    string nameWithStream;
    if("" == streamName)
    {
        nameWithStream = filename;
    }
    else
    {
        nameWithStream = filename + ":" + streamName;
    }

    hFile = CreateFile(nameWithStream.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL);

    if( hFile == INVALID_HANDLE_VALUE )
    {
        throw runtime_error("CreateFile() returned INVALID_HANDLE_VALUE");
    }
    else
    {
        WriteFile(hFile, content.c_str(), content.length(), &dwRet, NULL);
    }

    CloseHandle(hFile);
}

string Application::getStreamContent(const string & streamName) throw(runtime_error)
{
    cout << "Application::getStreamContent(" << streamName << ")" << endl;

    map<string, string>::iterator iFound = changedStreams.find(streamName);
    if(iFound != changedStreams.end())
    {
        return iFound->second;
    }
    else
    {

    }

    string content;

    HANDLE hFile;
    DWORD dwRet;

    string nameWithStream;
    if("" == streamName)
    {
        nameWithStream = filename;
    }
    else
    {
        nameWithStream = filename + ":" + streamName;
    }

    hFile = CreateFile(nameWithStream.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL);

    if(hFile == INVALID_HANDLE_VALUE)
    {
        throw runtime_error("CreateFile() returned INVALID_HANDLE_VALUE");
    }
    else
    {
        while(true)
        {
            char c;
            if(0 != ReadFile(hFile, & c, 1, &dwRet, NULL))
            {
                content += c;
            }
            else
            {
                break;
            }
        }
    }

    CloseHandle(hFile);

    return content;
}

void Application::addObserver(AppObserver * observer) throw(logic_error)
{
    if(NULL == observer)
    {
        throw logic_error("addObserver(NULL)");
    }

    observers.push_back(observer);
}

void Application::notifyObserversAboutStatus()
{
    for(vector<AppObserver *>::iterator iObserver = observers.begin();
        iObserver != observers.end(); ++iObserver)
    {
        (* iObserver)->statusChanged(this);
    }
}

void Application::notifyObserversAboutFileName()
{
    for(vector<AppObserver *>::iterator iObserver = observers.begin();
        iObserver != observers.end(); ++iObserver)
    {
        (* iObserver)->fileNameChanged(this);
    }
}
