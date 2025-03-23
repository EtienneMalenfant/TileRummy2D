#pragma once
#include <vector>
#include <game/tile.h>

class ITileList {
public:
    virtual void add(Tile* tile) = 0;
    virtual Tile* get(size_t index) const = 0;
    virtual void set(size_t index, Tile* tile) = 0;
    virtual void insert(size_t index, Tile* tile) = 0;
    virtual size_t size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual Tile* remove(size_t index) = 0;
    virtual Tile* removeShift(size_t index) = 0;
    
    virtual ~ITileList() = default;
};  

class ArrayList : public ITileList {
private:
    static const size_t CAPACITY = 13;
    Tile* _tiles[CAPACITY];
    size_t _size = 0;

    void updateSize();
public:
    ArrayList() = default;
    ~ArrayList() override;

    void add(Tile* tile) override;
    Tile* get(size_t index) const override;
    void set(size_t index, Tile* tile) override;
    void insert(size_t index, Tile* tile);
    size_t size() const override;
    bool isEmpty() const override;
    Tile* remove(size_t index) override;
    Tile* removeShift(size_t index) override;
};    
