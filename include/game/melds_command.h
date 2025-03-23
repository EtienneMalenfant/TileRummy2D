#pragma once
#include <list>

#include <game/meld.h>
#include <game/action.h>
#include <i_command.h>

class CreateMeldCommand : public ICommand {
private:
    IMeld* _newMeld;
    std::list<IMeld*>* _melds;
    static int _nextMeldUid;
public:
    CreateMeldCommand(std::list<IMeld*>* melds);
    ~CreateMeldCommand() override = default;
    void execute() override;
    void undo() override;
    IMeld* getMeld();
};

class DeleteMeldCommand : public ICommand {
private:
    IMeld* _meld;
    std::list<IMeld*>* _melds;
    bool _hasUndo {false};
public:
    DeleteMeldCommand(IMeld* meld, std::list<IMeld*>* melds);
    ~DeleteMeldCommand() override;
    void execute() override;
    void undo() override;
};

class MoveTileCommand : public ICommand {
protected:
    IMeld* _srcMeld;
    IMeld* _destMeld;
    Action* action;

    Tile* _tile {};
    int _srcIndex {-1};
    int _destIndex {-1};

    int getDestIndex();
public:
    MoveTileCommand(IMeld* srcMeld, IMeld* destMeld, Action* action);
    ~MoveTileCommand() override;
    void execute() override;
    void undo() override;
};

// SmartMoveTile se charge de fermer les trou si c'est pertinent
class SmartMoveTileCommand : public MoveTileCommand {
private:
    bool _hasShiftedTiles {false};
public:
    SmartMoveTileCommand(IMeld* srcMeld, IMeld* destMeld, Action* action);
    void execute() override;
    void undo() override;
};


class PutTileCommand : public ICommand {
private:
    Tile* _tile;
    IMeld* _meld;
    Action* action;

    int _index {0};
public:
    PutTileCommand(Tile* tile, IMeld* meld, Action* action);
    ~PutTileCommand() override;
    void execute() override;
    void undo() override;
};

