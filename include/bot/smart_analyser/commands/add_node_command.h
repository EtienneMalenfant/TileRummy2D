#pragma once
#include <i_command.h>
#include <bot/smart_analyser/tile_node.h>

namespace bot {

    class AddNodeCommand : public ICommand {
    private:
        ICommand* _linkNodeCommand {nullptr};
        ICommand* _assignJokerCommand {nullptr};
    public:
        AddNodeCommand(ITileNode* node, std::vector<ITileNode*>* meld, ValueDirection direction);
        ~AddNodeCommand() override;
        void execute() override;
        void undo() override;
    };
}
