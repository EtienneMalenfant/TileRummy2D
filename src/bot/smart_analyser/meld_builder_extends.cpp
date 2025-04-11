#include <bot/smart_analyser/meld_builder.h>

using namespace bot;

// * Séparation des définitions de TileNodeIterator parce que ça devient gros

void filterJokers(std::vector<ITileNode*>* compatibleNodes, ITileNode* node) {
    auto it = compatibleNodes->begin();
    while (it != compatibleNodes->end()) {
        ITileNode* compatibleNode = *it;
        if (compatibleNode->getTileWrapper()->tile->isJoker && compatibleNode->getTileWrapper()->tile->value != node->getTileWrapper()->tile->value) {
            it = compatibleNodes->erase(it);
            continue;
        }
        it++;
    }
}

void filterSets(std::vector<ITileNode*>* compatibleNodes) {
    auto it = compatibleNodes->begin();
    while (it != compatibleNodes->end()) {
        ITileNode* compatibleNode = *it;
        // on ignore les sets plein ou les tuiles seules ou les tuiles ne faisant pas partie d'un set
        if (compatibleNode->getMeldTileCount() >= 4 || compatibleNode->getMeldTileCount() < 2 || compatibleNode->evaluateMeldType() != MeldType::SET) {
            it = compatibleNodes->erase(it);
            continue;
        }
        it++;
    }
}

std::vector<Iterator<ITileNode*>*> getMeldsIterators(std::vector<ITileNode*>* compatibleNodes) {
    std::vector<Iterator<ITileNode*>*> nodeIterators;
    for (ITileNode* compatibleNode : *compatibleNodes) {
        // vérifier qu'on a pas déjà un itérateur de ce meld
        Iterator<ITileNode*>* currentIterator = compatibleNode->getMeldIterator();
        bool hasIterator = false;
        for (Iterator<ITileNode*>* otherIterator : nodeIterators) {
            if (otherIterator->peek() == currentIterator->peek()) {
                hasIterator = true;
                delete currentIterator;
                currentIterator = nullptr;
                break;
            }
        }
        if (hasIterator == false) {
            nodeIterators.push_back(currentIterator);
        }
    }
    return nodeIterators;
}

bool MeldBuilder::extendSet(ITileNode* node) {
    if (node->getTileWrapper()->tile->isJoker) {
        return false;
    }
    std::vector<ITileNode*>* compatibleNodes = copyCompatibleNodes(node, ValueDirection::Same);
    // s'il n'y a pas assez de tuiles compatibles
    if (compatibleNodes->size() < 3) {
        return false;
    }
    filterJokers(compatibleNodes, node);
    filterSets(compatibleNodes);
    // s'il ne reste plus asser de tuiles compatibles
    if (compatibleNodes->size() < 3) {
        delete compatibleNodes;
        return false;
    }

    std::vector<Iterator<ITileNode*>*> meldsIterators = getMeldsIterators(compatibleNodes);
    // vérifier que la couleur de notre tuile n'est pas présente
    for (Iterator<ITileNode*>* iterator : meldsIterators) {
        bool hasSameColor = false;
        ITileNode* meldNode;
        while (iterator->hasNext()) {
            meldNode = iterator->next();
            if (meldNode->getTileWrapper()->tile->color == node->getTileWrapper()->tile->color) {
                hasSameColor = true;
                break;
            }
        }
        delete iterator;

        if (hasSameColor == false) {
            ITileGroup* insertionGroup = new InsertionGroup(node, meldNode, nullptr);
            node->linkMeldTile(meldNode, MeldDirection::Previous);
            _currentGameState->add(insertionGroup);
            delete compatibleNodes;
            return true;
        }
    }
    delete compatibleNodes;
    return false;
}

bool MeldBuilder::extendSequence(ITileNode* node) {
    std::vector<ITileNode*> lowerValueNodes = node->getCompatibleNodes(ValueDirection::Lower);
    std::vector<ITileNode*> higherValueNodes = node->getCompatibleNodes(ValueDirection::Higher);
    // s'il n'y a pas assez de tuiles compatibles
    if (lowerValueNodes.size() < 1 && higherValueNodes.size() < 1) {
        return false;
    }
    // vérifier si on peut extends dans une des extrémités
    for (ITileNode* lowerValueNode : lowerValueNodes) {
        // au moins 1 tuiles à gauche et 0 à droite (1 tuile + la tuile présente = 2 tuiles dans le meld)
        if (lowerValueNode->getMeldTileCount(MeldDirection::Previous) >= 1
            && lowerValueNode->getMeldTileCount(MeldDirection::Next) == 0
            && lowerValueNode->evaluateMeldType() == MeldType::SEQUENCE
            && lowerValueNode->getTileWrapper()->tile->value == node->getTileWrapper()->tile->value - 1
            && lowerValueNode->getTileWrapper()->tile->color == node->getTileWrapper()->tile->color)
        {
            ITileGroup* insertionGroup = new InsertionGroup(node, lowerValueNode, nullptr);
            node->linkMeldTile(lowerValueNode, MeldDirection::Previous);
            _currentGameState->add(insertionGroup);
            return true;
        }
    }
    for (ITileNode* higherValueNode : higherValueNodes) {
        // au moins 1 tuiles à droite et 0 à gauche
        if (higherValueNode->getMeldTileCount(MeldDirection::Next) >= 1
            && higherValueNode->getMeldTileCount(MeldDirection::Previous) == 0
            && higherValueNode->evaluateMeldType() == MeldType::SEQUENCE
            && higherValueNode->getTileWrapper()->tile->value == node->getTileWrapper()->tile->value + 1
            && higherValueNode->getTileWrapper()->tile->color == node->getTileWrapper()->tile->color)
        {
            ITileGroup* insertionGroup = new InsertionGroup(node, nullptr, higherValueNode);
            node->linkMeldTile(higherValueNode, MeldDirection::Next);
            _currentGameState->add(insertionGroup);
            return true;
        }
    }
    return false;
}
