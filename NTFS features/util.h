#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

string getEditBoxString(HWND hEditBox)
{
    HLOCAL localMemory = reinterpret_cast<HLOCAL>(SendMessage(hEditBox, EM_GETHANDLE, 0, 0));
    if(0 == localMemory)
    {
        cerr << "SendMessage(hEditBox, 0, 0) returned 0" << endl;
    }
    char * str = reinterpret_cast<char *>(LocalLock(localMemory));
    return str;
}

size_t getLeftPaneWidth(size_t windowWidth)
{
    windowWidth = windowWidth * 0.3;
    double a = 10;
    return (windowWidth / a + sin(windowWidth / a)) * a;
}

#endif // UTIL_H_INCLUDED
