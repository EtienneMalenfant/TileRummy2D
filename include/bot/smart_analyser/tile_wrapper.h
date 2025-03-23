#pragma once
#include <game/meld.h>

namespace bot {

    // Garde les informations d'une tuile (son meld, si c'est une tuile du joueur)
    class TileWrapper {
    private:
        const Tile* _currentTile;
        const Tile* _createdTile {nullptr};
    public:
        TileWrapper(const Tile* tile, const IMeld* meldWrapper, bool isPlayerTile);
        ~TileWrapper();
        const Tile* const & tile { _currentTile };
        const IMeld* const meld;
        const bool isPlayerTile;

        void setJokerValue(int value, Color color);
    };
}
