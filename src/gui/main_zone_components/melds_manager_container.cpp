#include <gui/main_zone_components/melds_manager_container.h>
#include <gui/main_zone_components/meld_container.h>

using namespace gui;

MeldsManagerContainer::MeldsManagerContainer(ITileFactory* tileFactory, const IMeldsContainer* meldsContainer, IPlayerController* playerController)
    : _playerController(playerController)
{
    _meldsById.reserve(16);
    _meldsFactory = new MeldContainersFactory(meldsContainer->getMelds(), &_meldsById, tileFactory, playerController);
}

MeldsManagerContainer::~MeldsManagerContainer() {
    for (ITileContainer* meldContainer : _meldsById) {
        delete meldContainer;
        meldContainer = nullptr;
    }
    for (int i = 0; i < _rowCapacity; i++) {
        delete _meldsIdMap[i];
    }
    delete[] _meldsIdMap;
}

// ------------------------------
// implémentation privée des méthodes de ITileContainer

int MeldsManagerContainer::getRow(const sf::Vector2f& position) {
    return (position.y - _topBottomPadding) / (TileInfo::tileHeight + _rowSpacing);
}

ITileContainer* MeldsManagerContainer::getMeldAt(const sf::Vector2f& position) {
    int row = getRow(position);
    if (row >= 0 && row < _rowCapacity) {
        for (int meldId : *_meldsIdMap[row]) {
            ITileContainer* meld = _meldsById[meldId];
            if (meld->getGlobalBounds().contains(position)) {
                return meld;
            }
        }
    }
    return nullptr;
}

ITileContainer* MeldsManagerContainer::getNearMeld(const sf::Vector2f& position) {
    int row = getRow(position);
    if (row >= 0 && row < _rowCapacity) {
        for (int meldId : *_meldsIdMap[row]) {
            sf::FloatRect meldBounds = _meldsById[meldId]->getGlobalBounds();
            // le clique se situe a une distance d'au moin une tuile du meldContainer
            if (position.x > meldBounds.left - TileInfo::tileWidth && position.x < (meldBounds.left + meldBounds.width + TileInfo::tileWidth)) {
                return _meldsById[meldId];
            }
        }
    }
    return nullptr;
}

// ITileContainer publique

Tile2D* MeldsManagerContainer::getTileAt(const sf::Vector2f& position) {
    ITileContainer* meld = getMeldAt(position);
    if (meld != nullptr) {
        return meld->getTileAt(position);
    }
    return nullptr;
}


bool MeldsManagerContainer::addTileAt(Tile2D* tile, const sf::Vector2f& position) {
    // on cherche un meld a une distance de moin d'une tuile de la position
    ITileContainer* meld = getNearMeld(position);

    // si on a trouvé un meld
    if (meld != nullptr) {
        return meld->addTileAt(tile, position);
    }
    return _playerController->addAction(new Action{tile->getTileData()->uid, -1, -1});
}

void MeldsManagerContainer::previewTileAt(sf::Transformable& placeholder, const sf::Vector2f& position) {
    ITileContainer* meld = getNearMeld(position);
    if (meld != nullptr) {
        meld->previewTileAt(placeholder, position);
    }
    else {
        // on place le placeholder à la position du clic
        placeholder.setPosition({-1000, -1000});
    }
}

// ------------------------------
// implémentation privée de la méthode update()

void MeldsManagerContainer::placeMeld(int meldContainerId) {
    // trouver une rangée disponible
    for (int i = 0; i < _rowCapacity; i++) {
        int meldCount = _meldsIdMap[i]->size();
        // il y moins de N melds dans la rangée
        if (meldCount < 4) {
            bool hasPlacedMeld = couldPlaceMeld(i, meldContainerId);
            if (hasPlacedMeld) {
                return;
            }
        }
    }
    throw new std::runtime_error("Soft lock : il n'y a plus de place pour les melds");
    return;
}

// appelé seulement par placeMeld( )
bool MeldsManagerContainer::couldPlaceMeld(int row, int meldContainerId) {
    float nextXPos = Container::getPosition().x + _leftRightPadding;
    sf::FloatRect meldToPlaceBounds = _meldsById[meldContainerId]->getGlobalBounds();
    // si le meld est vide, on calcule pour qu'il rentre au moins N tuiles
    meldToPlaceBounds.width = std::max(meldToPlaceBounds.width, TileInfo::tileWidth * 3.f);
    std::vector<int>* meldList = _meldsIdMap[row];

    const float meldStartPosition = Container::getPosition().x + _leftRightPadding;
    for (int i = 0; i <= meldList->size(); i++) {

        float availableSpace = getAvailableSpace(meldList, i, nextXPos);

        // si l'espace restant est suffisant
        if (availableSpace >= meldToPlaceBounds.width) {
            // placer le meld
            float y = row * (TileInfo::tileHeight + _rowSpacing) + _topBottomPadding;
            _meldsById[meldContainerId]->setPosition(nextXPos, y);
            _meldsIdMap[row]->insert(meldList->begin() + i, meldContainerId);
            return true;
        }

        if (i < meldList->size()) {
            sf::FloatRect nextMeldBounds = _meldsById[meldList->at(i)]->getGlobalBounds();
            nextXPos = nextMeldBounds.width + nextMeldBounds.left + _colSpacing;
        }
    }

    return false;
}

// appelé seulement par couldPlaceMeld( )
float MeldsManagerContainer::getAvailableSpace(std::vector<int>* meldIdList, int index, float nextXPos) {
    float availableSpace = -1;
    // pour le cas où il n'y a plus de meld après i
    if (index == meldIdList->size()) {
        sf::FloatRect parentBounds = Container::getGlobalBounds();
        // obtenir la coord X du dernier objet de la rangée
        float lastMeldXEnd = parentBounds.left + _leftRightPadding;
        if (meldIdList->size() > 0) {
            sf::FloatRect lastMeldBounds = _meldsById[meldIdList->back()]->getGlobalBounds();
            lastMeldXEnd = lastMeldBounds.width + lastMeldBounds.left;
        }

        float xCoordLimit = parentBounds.width + parentBounds.left - _leftRightPadding;
        availableSpace = xCoordLimit - lastMeldXEnd + _colSpacing;
    }
    // on calcule l'espace disponible jusqu'au prochain meld
    else {
        float nextMeldLeftPos = _meldsById[meldIdList->at(index)]->getPosition().x;
        availableSpace = nextXPos - nextMeldLeftPos;
        availableSpace -= _colSpacing; // pour garder le spacing entre les melds
    }
    return availableSpace;
}
// - - - -

// appelé par update() et repositionExistingMelds()
void MeldsManagerContainer::repositionExistingMelds() {
    // tasser les melds s'il y en a un qui a grossi
    for (int row = 0; row < _rowCapacity; row++) {
        float lastMeldXEndPos = -999;
        // pour tous les meldContainer de la rangée
        for (int i = 0; i < _meldsIdMap[row]->size(); i++) {

            ITileContainer* meldContainer = _meldsById[_meldsIdMap[row]->at(i)];
            // s'il n'y a pas au moins _colSpacing de distance entre les melds
            if ((meldContainer->getPosition().x - lastMeldXEndPos) < _colSpacing) {
                shiftMelds(row, i, lastMeldXEndPos);
            }
            lastMeldXEndPos = meldContainer->getGlobalBounds().left + meldContainer->getGlobalBounds().width;
        }

        // s'il y a au moins 1 meld dans la rangée
        if (_meldsIdMap[row]->size() > 1) {
            // on regarde s'il y un meld qui va trop loin (qui dépasse la zone)
            handleMeldOverflow(_meldsIdMap[row], lastMeldXEndPos);
        }
    }
}

// appelé par repositionExistingMelds()
void MeldsManagerContainer::shiftMelds(int row, int index, float lastMeldXEndPos) {
    int meldId = _meldsIdMap[row]->at(index);
    float y = _meldsById[meldId]->getPosition().y;
    for (int i = index; i < _meldsIdMap[row]->size(); i++) {
        meldId = _meldsIdMap[row]->at(i);
        ITileContainer* meld = _meldsById[meldId];
        meld->setPosition(lastMeldXEndPos + _colSpacing, y);
        lastMeldXEndPos = meld->getGlobalBounds().left + meld->getGlobalBounds().width;
    }
}

// appelé par repositionExistingMelds()
void MeldsManagerContainer::handleMeldOverflow(std::vector<int>* row, float lastMeldXEndPos) {
    // Si le dernier meld dépasse de la zone du container
    sf::FloatRect parentContainer = Container::getGlobalBounds();
    if (lastMeldXEndPos > ((parentContainer.left + parentContainer.width) - _leftRightPadding)) {
        int meldToMoveId = row->back();
        ITileContainer* meldToMove = _meldsById[meldToMoveId];
        placeMeld(meldToMoveId);
        row->erase(row->end() - 1);
    }
}
// - - - -

// IUpdateable publique

void MeldsManagerContainer::update() {
    // on se met à jour avec le jeu
    _meldsFactory->createNewMeldContainers();
    _meldsFactory->deleteOldMeldContainers(_meldsIdMap, _rowCapacity);

    // passer sur tous les melds du jeu pour les placer si besoin et les mettre à jour
    for (int i = 0; i < _meldsById.size(); i++) {
        ITileContainer* meldContainer = _meldsById[i];
        if (meldContainer != nullptr) {
            // placer les melds nouvellement créés
            if (meldContainer->getPosition().x <= 0) {
                placeMeld(i);
            }
            // update tous les melds
            static_cast<IUpdateable*>(meldContainer)->update(); // cast parce que update() est aussi sur sf::Shape
        }
    }
    // passer sur les melds de la map pour les déplacer si besoin
    repositionExistingMelds();
}


// ------------------------------
// sf::Shape publique

void MeldsManagerContainer::setSize(const sf::Vector2f& size) {
    Container::setSize(size);
    // calculer le nombre de rangées
    _rowCapacity = (size.y - _topBottomPadding * 2) / (TileInfo::tileHeight + _rowSpacing);
    // créer les rangées
    _meldsIdMap = new std::vector<int> * [_rowCapacity];
    for (int row = 0; row < _rowCapacity; row++) {
        _meldsIdMap[row] = new std::vector<int>();
    }
}

void MeldsManagerContainer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Container::draw(target, states);
    for (ITileContainer* meld : _meldsById) {
        if (meld != nullptr) {
            target.draw(*meld, states);
        }
    }
}
