#pragma once
#include <vector>
#include <game/tile.h>
#include <game/tile_list.h>
#include <game/meld_validator.h>

enum class MeldType {
    SEQUENCE, // une série de la même couleur
    SET, // un groupe de 3 ou 4 tuiles de la même valeur, couleur différente
    INVALID
};

class IMeld : public ITileList {
public:
    virtual MeldType getType() const = 0;
    virtual bool isValid() const = 0;
    virtual int findByUID(int uid) const = 0;
    virtual int getId() const = 0;
    virtual ~IMeld() = default;
};

static IMeldValidatorFactory* validatorFactory {new MeldValidatorFactory()};

class Meld : public IMeld  {
private:
    MeldType _type {MeldType::INVALID};
    ITileList* _tiles {};
    IMeldValidator* _validator {};
    const int uid;

    void update();
    void updateType();
    void checkForJoker();
    bool validateIntegrity();
public:
    Meld(int id = -1);
    ~Meld() override;

    MeldType getType() const override;
    bool isValid() const override;
    int findByUID(int uid) const;
    int getId() const override;

    void add(Tile* item) override;
    Tile* get(size_t index) const override;
    void set(size_t index, Tile* item) override;
    void insert(size_t index, Tile* item) override;
    size_t size() const override;
    bool isEmpty() const override;
    Tile* remove(size_t index) override;
    Tile* removeShift(size_t index) override;

};