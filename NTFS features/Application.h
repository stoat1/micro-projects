#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <stdexcept>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Application;

class AppObserver
{
public:
    virtual ~AppObserver() {}
    virtual void fileNameChanged(Application * source) = 0;
    virtual void statusChanged(Application * source) = 0;
};

class Application
{
public:
    Application();

    void newFile() throw(runtime_error, logic_error);
    void openFile(const string & fileName) throw(runtime_error, logic_error);
    void saveFile(const string & fileName) throw(runtime_error, logic_error);
    void saveFile() throw(runtime_error);
    void closeFile() throw(runtime_error);

    bool isFileOpened() const;
    bool isFileSaved() const;
    string getCurentFileName() const;

    string getStreamContent(const string & streamName) throw(runtime_error);
    void setStreamContent(const string & streamName, const string & content) throw(runtime_error);

    void addObserver(AppObserver * observer) throw(logic_error);
    void removeObserver(AppObserver * observer) throw(logic_error);

private:

    void notifyObserversAboutStatus();
    void notifyObserversAboutFileName();

    string filename;
    map<string, string> changedStreams;
    vector<AppObserver *> observers;
    bool opened;
    bool saved;
};

#endif // APPLICATION_H_INCLUDED
