#pragma once
#include <stack>

class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~ICommand() = default;

    static void cancelCommands(std::stack<ICommand*>& commandStack);
};

