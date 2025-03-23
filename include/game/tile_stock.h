#pragma once
#include <random>
#include <game/tile.h>
#include <logger.h>

class ITileStock {
public:
    virtual Tile* draw() = 0;
    virtual ~ITileStock() = default;
};

class TileStock : public ITileStock {
private:
    ILogger* _logger {nullptr};
    unsigned int _seed {0};
    int _lastTileIndex {105};
    Tile* _tiles[106] {};
    std::mt19937 _randGen;

    void buildStock();
public:
    TileStock();
    TileStock(ILogger* logger);
    ~TileStock() override;
    void setSeed(unsigned int seed);
    Tile* draw() override;
};
