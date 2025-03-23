#include <stdexcept>
#include <game/tile_list.h>
#include <game/tile.h>


ArrayList::~ArrayList() {
    for (size_t i = 0; i < _size; i++) {
        delete _tiles[i];
    }
}

void ArrayList::add(Tile* tile) {
    if (_size >= CAPACITY) {
        throw std::out_of_range("List is full");
    }
    _tiles[_size] = tile;
    _size++;
}

Tile* ArrayList::get(size_t index) const {
    if (index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return _tiles[index];
}

void ArrayList::set(size_t index, Tile* tile) {
    if (index >= CAPACITY) {
        throw std::out_of_range("Index out of range");
    }
    _size = std::max(_size, index + 1);
    _tiles[index] = tile;
}

void ArrayList::insert(size_t index, Tile* tile) {
    if (index > CAPACITY) {
        throw std::out_of_range("Index out of range");
    }

    for (size_t i = _size; i > index; i--) {
        _tiles[i] = _tiles[i - 1];
    }
    _tiles[index] = tile;
    updateSize();
}

size_t ArrayList::size() const {
    return _size;
}

bool ArrayList::isEmpty() const {
    return _size == 0;
}

Tile* ArrayList::remove(size_t index) {
    if (index >= CAPACITY) {
        throw std::out_of_range("Index out of range");
    }

    Tile* removedTile = _tiles[index];
    _tiles[index] = nullptr;
    updateSize();

    return removedTile;
}

Tile* ArrayList::removeShift(size_t index) {
    if (index >= CAPACITY) {
        throw std::out_of_range("Index out of range");
    }

    Tile* removedTile = _tiles[index];
    // je me rend à size -1 pour ne pas déborder si le tableau est plein
    for (size_t i = index; i < _size - 1; i++) { 
        _tiles[i] = _tiles[i + 1];
    }
    _tiles[_size - 1] = nullptr;
    updateSize();

    return removedTile;
}

// ------------------------------

void ArrayList::updateSize() {
    int lastIndex {-1};
    for (int i{0}; i < CAPACITY; i++) {
        if (_tiles[i] != nullptr) {
            lastIndex = i;
        }
    }
    _size = lastIndex + 1;
}