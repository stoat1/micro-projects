#include "ContentPane.h"

#include <sstream>
#include <iostream>

#include "ContentPane.h"
#include "SerialGenerator.h"
#include "Context.h"

using namespace std;

ContentPane::~ContentPane()
{}

void ContentPane::clear()
{
    saveToConfig(* config);

    cerr << "In ContentPane::clear()" << endl;
    for(size_t i = 0; i < items.size(); ++i)
    {
        DestroyWindow(items.at(i).first);
        DestroyWindow(items.at(i).second);
    }
    items.clear();

    config = NULL;
}

void ContentPane::clear(Config & conf)
{
    cerr << "In ContentPane::clear(Conf)" << endl;
    SerialGenerator & gen = SerialGenerator::getInstance();

    clear();

    config = & conf;
    const Config & defaultConfig = Config::getDefaultLocalization();

    int y = 2 * V_INDENT + BUTTON_HEIGHT;
    int x = 2 * H_INDENT + LISTBOX_WIDTH;

    for(map<string, string>::iterator iItem = config->getMap().begin();
        iItem != config->getMap().end(); ++iItem)
    {
        // not to process superfluous item
        if(0 == defaultConfig.getMap().count(iItem->first))
        {
            continue;
        }

        HMENU id = gen();

        HWND label = CreateWindow(
            "static",
            iItem->first.c_str(),
            WS_CHILD | WS_VISIBLE,
            x, y, LABEL_WIDTH, LINE_HEIGHT,
            owner->getHwnd(),
            id,
            owner->getHinstance(),
            NULL
        );

        id = gen();

        HWND edit = CreateWindow(
            "edit",
            iItem->second.c_str(),
            WS_CHILD | WS_VISIBLE,
            x + LABEL_WIDTH, y, FIELD_WIDTH, LINE_HEIGHT,
            owner->getHwnd(),
            id,
            owner->getHinstance(),
            NULL
        );

        items.push_back(make_pair(label, edit));

        y += H_INDENT + LINE_HEIGHT;
    }
}

void ContentPane::saveToConfig(Config & config)
{
    cerr << "In ContentPane::saveToConfig()" << endl;
    for(size_t i = 0; i < items.size(); ++i)
    {
        size_t BUFFER_SIZE = 1000;
        char buffer[BUFFER_SIZE];
        string key;
        string value;

        GetWindowText(items.at(i).first, buffer, BUFFER_SIZE);
        key = buffer;

        GetWindowText(items.at(i).second, buffer, BUFFER_SIZE);
        value = buffer;

        config.getMap()[key] = value;
    }
}

ContentPane::ContentPane(MainWindow * owner)
: owner(owner)
, config(NULL)
, items()
{

}
