#include "Config.h"
#include <windows.h>
#include <sstream>
#include <iostream>

Config::Config()
: mapping()
{}

void Config::loadFromFileStream(
    const string & fileName,
    const string & streamName
) throw(runtime_error)
{
    HANDLE hStream;
    string fileStreamName;

    if(streamName.empty())
    {
        fileStreamName = fileName;
    }
    else
    {
        fileStreamName = fileName + ":" + streamName;
    }

    try
    {
        // open file stream
        hStream = CreateFile(
            fileStreamName.c_str() /* file name */,
            GENERIC_READ /* desired access */,
            0 /* share mode */,
            NULL /* security attributes */,
            OPEN_EXISTING /* creation disposition */,
            FILE_ATTRIBUTE_NORMAL /* flags and attributes */,
            NULL /* template file */
        );
        if(INVALID_HANDLE_VALUE == hStream)
        {
            ostringstream osstr;
            osstr << "Cannot open file " << fileStreamName
                << ", error code is " << GetLastError();
            throw runtime_error(osstr.str());
        }

        // read
        bool result = true;
        while(result)
        {
            string str;
            result = readString(hStream, str);
            if(result)
            {
                string key;
                string value;

                if(splitLine(str, key, value))
                {
                    mapping[key] = value;
                }
            }
        }

        CloseHandle(hStream);
    }
    catch(runtime_error & ex)
    {
        cerr << "runtime_error: " << ex.what() << endl;
    }

    cerr << * this << endl;
//    addMissing();
}

void Config::saveToFileStream(
    const string & fileName,
    const string & streamName
) throw(runtime_error)
{
    HANDLE hStream;
    string fileStreamName;

    if(streamName.empty())
    {
        fileStreamName = fileName;
    }
    else
    {
        fileStreamName = fileName + ":" + streamName;
    }

    // open file stream
    hStream = CreateFile(
        fileStreamName.c_str() /* file name */,
        GENERIC_WRITE /* desired access */,
        FILE_SHARE_WRITE /* share mode */,
        NULL /* security attributes */,
        CREATE_ALWAYS /* creation disposition */,
        FILE_ATTRIBUTE_NORMAL /* flags and attributes */,
        NULL /* template file */
    );
    if(INVALID_HANDLE_VALUE == hStream)
    {
        ostringstream osstr;
        osstr << "Cannot open file " << fileStreamName
            << ", error code is " << GetLastError();
        throw runtime_error(osstr.str());
    }

    // write
    for(map<string, string>::const_iterator iItem = mapping.begin();
        iItem != mapping.end(); ++iItem)
    {
        BOOL bResult;
        DWORD nNumberOfBytesWritten;
        string strItem;

        if(iItem != mapping.begin())
        {
            strItem = "\n" + iItem->first + "=" + iItem->second;
        }
        else
        {
            strItem = iItem->first + "=" + iItem->second;
        }

        bResult = WriteFile(
            hStream,
            strItem.c_str(),
            strItem.length(),
            & nNumberOfBytesWritten,
            NULL
        );
        if(FALSE == bResult)
        {
            ostringstream osstr;
            osstr << "Cannot write to file " << fileStreamName
                << ", error code is " << GetLastError();
            throw runtime_error(osstr.str());
        }
    }

    CloseHandle(hStream);
}

void Config::addMissing()
{
    const Config & defaultConf = getDefaultLocalization();
    const map<string, string> & confMap = defaultConf.getMap();

    for(map<string, string>::const_iterator iter = confMap.begin();
        iter != confMap.end(); ++iter)
    {
        if(0 == mapping.count(iter->first))
        {
            // add missing item
            mapping[iter->first] = iter->second;
        }
    }
}

const Config & Config::getDefaultLocalization()
{
    static Config * defaultLocal = NULL;

    if(NULL == defaultLocal)
    {
        defaultLocal = new Config();
        defaultLocal->mapping.insert(make_pair("NEW_BUTTON", "NEW"));
        defaultLocal->mapping.insert(make_pair("OPEN_BUTTON", "OPEN"));
        defaultLocal->mapping.insert(make_pair("SAVE_BUTTON", "SAVE"));
        defaultLocal->mapping.insert(make_pair("SAVE_AS_BUTTON", "SAVE AS"));
        defaultLocal->mapping.insert(make_pair("EXIT_BUTTON", "EXIT"));
        defaultLocal->mapping.insert(make_pair("CLOSE_BUTTON", "CLOSE"));
    }

    return * defaultLocal;
}

map<string, string> & Config::getMap()
{
    return mapping;
}

const map<string, string> & Config::getMap() const
{
    return mapping;
}

bool Config::splitLine(const string & line, string & key, string & value)
{
    size_t found;

    found = line.find('=');
    if(string::npos == found)
    {
        return false;
    }

    key = line.substr(0, found);
    value = line.substr(found + 1);

    return true;
}

bool Config::readString(HANDLE hFile, string & str)
{
    BOOL bResult;
    CHAR cBuffer;
    DWORD nNumberOfBytesRead;

    str.clear();

    while(true)
    {
        bResult = ReadFile(hFile, & cBuffer, 1, & nNumberOfBytesRead, NULL);
        if((FALSE == bResult) || (0 == nNumberOfBytesRead))
        {
            break;
        }
        else if('\n' == cBuffer)
        {
            break;
        }
        else
        {
            str += cBuffer;
        }
    }

    if(str.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

ostream & operator<<(ostream & ost, const Config & config)
{
    const map<string, string> mapping = config.getMap();

    for(map<string, string>::const_iterator iter = mapping.begin();
        iter != mapping.end(); ++iter)
    {
        ost << iter->first << "=" << iter->second << endl;
    }

    return ost;
}
