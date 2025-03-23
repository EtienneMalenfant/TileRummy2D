#pragma once
#include <i_command.h>
#include <bot/smart_analyser/tile_node.h>

namespace bot {

    class AddNodeCommand : public ICommand {
    private:
        ICommand* _linkNodeCommand;
        ICommand* _assignJokerCommand {nullptr};
    public:
        AddNodeCommand(ITileNode* node, std::vector<ITileNode*>* meld, ValueDirection direction);
        void execute() override;
        void undo() override;
    };
}
