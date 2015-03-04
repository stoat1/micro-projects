#ifndef DIALOGS_H_INCLUDED
#define DIALOGS_H_INCLUDED

#include "windows.h"
#include <string>
#include <stdexcept>

using namespace std;

pair<bool, string> getOpenFileName(HWND hwndOwner);
pair<bool, string> getSaveFileName(HWND hwndOwner);

#endif // DIALOGS_H_INCLUDED
