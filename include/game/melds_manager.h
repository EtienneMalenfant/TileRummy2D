#pragma once
#include <list>
#include <stack>

#include <game/meld.h>
#include <game/action.h>
#include <i_command.h>


class IMeldsManager {
public:
    virtual bool confirm() = 0;
    virtual void cancel() = 0;
    virtual void moveTile(Action* action) = 0;
    virtual void putTile(Action* action, Tile* tile) = 0;
    virtual ~IMeldsManager() = default;
};

class IMeldsContainer {
public :
    virtual const std::list<const IMeld*>* getMelds() const = 0;
    virtual ~IMeldsContainer() = default;
};

class MeldsManager : public IMeldsManager, public IMeldsContainer {
private:
    std::list<IMeld*> _melds;
    std::stack<ICommand*> _executedActions;

    void clearActions();
    bool validateAction(Action* action);
    IMeld* getDestMeld(Action* action);
    IMeld* findMeld(int tileUID);
    void cleanUpMeld(IMeld* meld);
    void splitMeld(IMeld* meld, int index);
public:
    MeldsManager() = default;
    ~MeldsManager() override;
    bool confirm() override;
    void cancel() override;
    void moveTile(Action* action) override;
    void putTile(Action* action, Tile* tile) override;
    const std::list<const IMeld*>* getMelds() const override;
};
