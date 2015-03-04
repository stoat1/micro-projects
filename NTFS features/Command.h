#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

class Command
{
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

class DebugCommand : public Command
{
public:

    DebugCommand(const string & text)
    : text(text)
    {}

    virtual void execute()
    {
        cerr << text << endl;
    }

    virtual ~DebugCommand()
    {}

private:

    string text;
};

#endif // COMMAND_H_INCLUDED
