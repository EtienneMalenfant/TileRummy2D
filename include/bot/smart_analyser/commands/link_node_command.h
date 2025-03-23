#pragma once
#include <i_command.h>
#include <bot/smart_analyser/tile_node.h>

namespace bot {

    class LinkNodeCommand : public ICommand {
    private:
        ITileNode* _originalLeftNode {nullptr};
        ITileNode* _orignialRightNode {nullptr};
        ITileNode* _insertedNode;
        MeldDirection _direction;
        std::vector<ITileNode*>* _meld;
    public:
        LinkNodeCommand(ITileNode* insertedNode, std::vector<ITileNode*>* _meld, ValueDirection direction);
        void execute() override;
        void undo() override;
    };
}
