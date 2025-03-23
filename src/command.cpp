#include <i_command.h>

void ICommand::cancelCommands(std::stack<ICommand*>& commandStack) {
    while (!commandStack.empty()) {
        ICommand* command = commandStack.top();
        command->undo();
        commandStack.pop();
        delete command;
    }
}
