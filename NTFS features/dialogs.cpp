#include "dialogs.h"

pair<bool, string> getOpenFileName(HWND hwndOwner)
{
    const size_t maxFile = 256;
    LPTSTR fileName = new TCHAR[maxFile]; // FIXME use windows constant

    OPENFILENAME ofn;
    ZeroMemory(& ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwndOwner;
    ofn.lpstrFilter = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = maxFile;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = "Select a file for opening";
    ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if(0 == GetOpenFileName(& ofn))
    {
        return make_pair(false, "");
    }
    else
    {
        return make_pair(true, fileName);
    }
}

pair<bool, string> getSaveFileName(HWND hwndOwner)
{
    const size_t maxFile = 256;
    LPTSTR fileName = new TCHAR[maxFile]; // FIXME use windows constant

    OPENFILENAME ofn;
    ZeroMemory(& ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwndOwner;
    ofn.lpstrFilter = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = maxFile;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = "Select a file for opening";
    ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if(0 == GetSaveFileName(& ofn))
    {
        return make_pair(false, "");
    }
    else
    {
        return make_pair(true, fileName);
    }
}
