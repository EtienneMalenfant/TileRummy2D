#include <game/meld.h>

Meld::Meld(int id): uid(id) {
    _tiles = new ArrayList();
    _validator = validatorFactory->getDefault();
}

Meld::~Meld() {
    delete _tiles;
}

// ------------------------------
// Méthodes privées

void Meld::update() {
    if (!validateIntegrity()) {
        _type = MeldType::INVALID;
        return;
    };
    checkForJoker();
    updateType();
}

bool Meld::validateIntegrity() {
    if (_tiles->size() < 3) {
        return false;
    }
    for (int i = 0; i < _tiles->size(); i++) {
        if (_tiles->get(i) == nullptr) {
            return false;
        }
    }
    return true;
}

void Meld::updateType() {
    if (_tiles->size() < 3) {
        _type = MeldType::INVALID;
        return;
    }
    else if (_validator->isSequence(_tiles)) {
        _type = MeldType::SEQUENCE;
        return;
    } 
    else if (_validator->isSet(_tiles)) {
        _type = MeldType::SET;
        return;
    }
    _type = MeldType::INVALID;
}

void Meld::checkForJoker() {
    for (int i = 0; i < _tiles->size(); i++) {
        Tile* tile = _tiles->get(i);
        if (_tiles->get(i)->isJoker) {
            _validator = validatorFactory->getJokerValidator();
            return;
        }
    }
    _validator = validatorFactory->getDefault();
}

// ------------------------------
// Public

MeldType Meld::getType() const {
    return _type;
}

bool Meld::isValid() const {
    return _type != MeldType::INVALID;
}

int Meld::findByUID(int uid) const {
    for (int i = 0; i < _tiles->size(); i++) {
        Tile* tile = _tiles->get(i);
        if (tile == nullptr) {
            continue;
        }
        else if (tile->uid == uid) {
            return i;
        }
    }
    return -1;
}

int Meld::getId() const {
    return uid;
}

// ------------------------------

void Meld::add(Tile* item) {
    _tiles->add(item);
    update();
}

Tile* Meld::get(size_t index) const {
    return _tiles->get(index);
}

void Meld::set(size_t index, Tile* item) {
    _tiles->set(index, item);
    update();
}

void Meld::insert(size_t index, Tile* item) {
    _tiles->insert(index, item);
    update();
}

size_t Meld::size() const {
    return _tiles->size();
}

bool Meld::isEmpty() const {
    return _tiles->isEmpty();
}

Tile* Meld::remove(size_t index) {
    Tile* tile = _tiles->remove(index);
    update();
    return tile;
}

Tile* Meld::removeShift(size_t index) {
    Tile* tile = _tiles->removeShift(index);
    update();
    return tile;
}