#include <game/meld.h>

MeldValidatorFactory::MeldValidatorFactory() {
    _defaultValidator = new DefaultValidator();
    _jokerValidator = new ValidatorWithJoker();
}

MeldValidatorFactory::~MeldValidatorFactory() {
    delete _defaultValidator;
    delete _jokerValidator;
}

IMeldValidator* MeldValidatorFactory::getDefault() {
    if (_defaultValidator == nullptr) {
        _defaultValidator = new DefaultValidator();
    }
    return _defaultValidator;
}

IMeldValidator* MeldValidatorFactory::getJokerValidator() {
    if (_jokerValidator == nullptr) {
        _jokerValidator = new ValidatorWithJoker();
    }
    return _jokerValidator;
}

// ------------------------------

bool DefaultValidator::isSequence(ITileList* tiles) {
    Color color = tiles->get(0)->color;
    int lastValue = tiles->get(0)->value;
    for (int i = 1; i < tiles->size(); i++) {
        if (tiles->get(i)->color != color ||
            (tiles->get(i)->value) - lastValue != 1) {
            return false;
        }
        lastValue = tiles->get(i)->value;
    }
    return true;
}

bool DefaultValidator::isSet(ITileList* tiles) {
    if (tiles->size() < 3 || tiles->size() > 4) {
        return false;
    }
    bool presentColors[4] = {false};
    int value = tiles->get(0)->value;
    for (int i = 0; i < tiles->size(); i++) {
        Color color = tiles->get(i)->color;
        presentColors[(int)color] = !presentColors[(int)color];
        if (tiles->get(i)->value != value) {
            return false;
        }
    }

    int nbColors = 0;
    for (int i = 0; i < 4; i++) {
        if (presentColors[i]) {
            nbColors++;
        }
    }
    return nbColors >= 3;
}

// ------------------------------

Tile* ValidatorWithJoker::getFirstNonJoker(ITileList* tiles, int& tileIndex) {
    for (int i = 0; i < tiles->size(); i++) {
        if (!tiles->get(i)->isJoker) {
            tileIndex = i;
            return tiles->get(i);
        }
    }
    return nullptr;
}

// ------------------------------

bool ValidatorWithJoker::isSequence(ITileList* tiles) {
    //trouver la première tuile non-joker
    int firstTileIndex = -1;
    Tile* firstTile = getFirstNonJoker(tiles, firstTileIndex);

    // cas spéciale où la troisième tuiles est de plus petite valeure que 3
    if (firstTileIndex >= 2 && firstTile->value < 3) {
        return false;
    }
    int lastValue = firstTile->value;
    Color color = firstTile->color;

    Tile* jokers[2] {};
    int jokerValues[2] {-1};
    int jokerCount = 0;
    // si les jokers sont au début
    for (int i = 0; i < firstTileIndex; i++) {
        jokers[i] = tiles->get(i);
        jokerValues[i] = (lastValue - firstTileIndex) + i;
        jokerCount++;
    }
    // traitement pour les tuiles après la première tuile non-joker
    for (int i = firstTileIndex + 1; i < tiles->size(); i++) {
        Tile* t = tiles->get(i);
        //taitement pour les jokers non assignées
        if (t->isJoker) {
            jokers[jokerCount] = t;
            lastValue++;
            jokerValues[jokerCount] = lastValue;
            jokerCount++;
            continue;
        }
        //traitement pour les autres tuiles
        else if (t->color != color || t->value - 1 != lastValue) {
            return false;
        }
        lastValue = tiles->get(i)->value;
    }
    for (int i = 0; i < jokerCount; i++) {
        jokers[i]->setValue(jokerValues[i]);
        jokers[i]->setColor(color);
    }
    return true;
}


void emptyAvailableColors(Color** availableColors) {
    for (int i = 0; i < 2; i++) {
        delete availableColors[i];
    }
}


bool ValidatorWithJoker::isSet(ITileList* tiles) {
    if (tiles->size() < 3) {
        return false;
    }
    bool presentColors[4] = {false};
    Tile* jokers[2] {};
    int setValue = -1;
    int jokerCount = 0;
    for (int i = 0; i < tiles->size(); i++) {
        Tile* t = tiles->get(i);
        //traitement pour les jokers non assignés
        if (t->isJoker) {
            jokers[jokerCount] = t;
            jokerCount++;
            continue;
        }
        //traitement pour les autres tuiles
        if (setValue == -1) {
            setValue = tiles->get(i)->value;
        }
        Color color = tiles->get(i)->color;
        presentColors[(int)color] = !presentColors[(int)color];
        if (setValue != tiles->get(i)->value) {
            return false;
        }
    }

    //trouver le nombre de couleurs présentes
    int nbColors = 0;
    Color* availableColor[2] {nullptr};
    int availableColorCount = 0;
    for (int i = 0; i < 4; i++) {
        if (presentColors[i]) {
            nbColors++;
        }
        else if (availableColorCount < 2) {
            availableColor[availableColorCount] = new Color {i};
            availableColorCount++;
        }
    }
    if (nbColors >= (3 - jokerCount)) {
        for (int i = 0; i < jokerCount; i++) {
            jokers[i]->setValue(setValue); // le nombre du set
            jokers[i]->setColor(*availableColor[i]);
        }
        emptyAvailableColors(availableColor);
        return true;
    }
    emptyAvailableColors(availableColor);
    return false;
}
