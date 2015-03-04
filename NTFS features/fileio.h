#ifndef FILEIO_H_INCLUDED
#define FILEIO_H_INCLUDED

void loadFile(const string & filename) throw(runtime_error)
{
    cout << "Loading file \"" << filename << "\"" << endl;

    langs[0].content = "";
    langs[1].content = "";
    langs[2].content = "";
    langs[3].content = "";

    for(size_t iStream = 0; iStream < LANG_COUNT; ++iStream)
    {
        HANDLE hFile;
        size_t BUFFER_SIZE = 10;
        char buffer[BUFFER_SIZE];

        string fileAndStream;
        if(langs[iStream].streamName == "")
        {
            fileAndStream = filename;
        }
        else
        {
            fileAndStream = filename + ":" + langs[iStream].streamName;
        }

        hFile = CreateFile(
            fileAndStream.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if(INVALID_HANDLE_VALUE == hFile)
        {
            throw runtime_error("CreateFile() returned INVALID_HANDLE_VALUE");
        }

        while(true)
        {
            DWORD read;

            BOOL result = ReadFile(hFile, buffer, BUFFER_SIZE - 1, & read, NULL);
            if(FALSE == result)
            {
                cout << "FALSE GetLastError() == " << GetLastError() << endl;
                break;
            }

            if(read <= 0)
            {
                break;
            }

            langs[iStream].content += string(buffer).substr(0, read);

            cout << "read = " << read << endl;
        }

        CloseHandle(hFile);
    }
}

void saveFile(const string & filename) throw(runtime_error)
{
    cout << "Saving file \"" << filename << "\"" << endl;

    HANDLE hFile;

    for(size_t iStream = 0; iStream < LANG_COUNT; ++iStream)
    {
        string nameAndStream;

        if(langs[iStream].streamName == "")
        {
            nameAndStream = filename;
        }
        else
        {
            nameAndStream = filename + ":" + langs[iStream].streamName;
        }

        hFile = CreateFile(
            nameAndStream.c_str() /* lpFileName */,
            GENERIC_WRITE /* dwDesiredAccess */,
            0 /* dwShareMode */,
            NULL /* lpSecurityAttributes */,
            CREATE_ALWAYS /* dwCreationDisposition */,
            FILE_ATTRIBUTE_NORMAL /* dwFlagsAndAttributes */,
            NULL /* hTemplateFile */
        );

        if(INVALID_HANDLE_VALUE == hFile)
        {
            ostringstream osstr;
            osstr << "CreateFile(" << filename << ") returned INVALID_HANDLE_VALUE\n";
            osstr << "GetLastError() == " << GetLastError();
            throw runtime_error(osstr.str());
        }

        DWORD write;

        BOOL result = WriteFile(hFile, langs[iStream].content.c_str(), langs[iStream].content.length(), & write, NULL);
    //    BOOL result = WriteFile(hFile, "Hello, world", strlen("Hello, world"), & write, NULL);
        if(FALSE == result)
        {
            cout << "FALSE GetLastError() == " << GetLastError() << endl;
        }

        CloseHandle(hFile);
    }
}

#endif // FILEIO_H_INCLUDED
