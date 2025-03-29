#include <bot/smart_analyser/meld_builder.h>
#include <algorithm>
#include <bot/smart_analyser/commands/add_node_command.h>
#include <stdexcept>

using namespace bot;

std::vector<ITileNode*>* MeldBuilder::copyCompatibleNodes(ITileNode* node, ValueDirection d) {
    std::vector<ITileNode*>* newCompatibleNodes = new std::vector<ITileNode*>();
    const std::vector<ITileNode*> compatibleNodes = node->getCompatibleNodes(d);
    for (ITileNode* compatibleNode : compatibleNodes ) {
        newCompatibleNodes->push_back(compatibleNode);
    }
    return newCompatibleNodes;
}

bool MeldBuilder::meldContainTile(ITileNode* searchedNode, ITileNode* meldNode) {
    Iterator<ITileNode*>* iterator = meldNode->getMeldIterator();
    while (iterator->hasNext()) {
        ITileNode* current = iterator->next();
        if (current == searchedNode) {
            delete iterator;
            return true;
        }
    }
    delete iterator;
    return false;
}

std::vector<ITileNode*>* MeldBuilder::getSequenceExtensions(ITileNode* node, ValueDirection direction) {
    // quitter si ce n'est pas une séquence
    MeldType meldType = node->evaluateMeldType();
    if (meldType == MeldType::SET) {
        return new std::vector<ITileNode*>();
    }
    // sinon, on récupère les tuiles de la séquence dans la direction donnée
    Iterator<ITileNode*>* iterator = node->getMeldIterator();
    std::vector<ITileNode*>* sequenceExtension = new std::vector<ITileNode*>();
    if (direction == ValueDirection::Lower) {
        while (iterator->hasNext()) {
            ITileNode* current = iterator->next();
            if (current == node) {
                break;
            }
            if (current->getTileWrapper()->tile->value < node->getTileWrapper()->tile->value) {
                sequenceExtension->push_back(current);
            }
        }
    }
    else if (direction == ValueDirection::Higher) {
        bool isPastNode = false;
        while (iterator->hasNext()) {
            ITileNode* current = iterator->next();
            if (current == node) {
                isPastNode = true;
            }
            else if (isPastNode && current->getTileWrapper()->tile->value > node->getTileWrapper()->tile->value) {
                sequenceExtension->push_back(current);
            }
        }
    }
    delete iterator;
    return sequenceExtension;
}

std::vector<ITileNode*>* MeldBuilder::getOrphansFromRemoval(ITileNode* node, MeldType meldType) {
    Iterator<ITileNode*>* iterator = node->getMeldIterator();
    std::vector<ITileNode*>* orphans = new std::vector<ITileNode*>();
    if (node->getMeldTileCount() == 1) {
        return orphans;
    }
    // - Les Sets -
    else if (meldType == MeldType::SET) {
        if (node->getMeldTileCount() == 4) {
            return orphans; // rien à retourner, le set reste valide
        }
        while (iterator->hasNext()) {
            ITileNode* current = iterator->next();
            if (current != node) {
                orphans->push_back(current);
            }
        }
    }
    // - Les séquences -
    else if (meldType == MeldType::SEQUENCE) {
        int lowerCount = node->getMeldTileCount(MeldDirection::Previous);
        int higherCount = node->getMeldTileCount(MeldDirection::Next);
        // s'il reste 3 tuiles d'un bord et 0 de l'autre
        if ((lowerCount == 0 && higherCount >= 3) || (higherCount == 0 && lowerCount >= 3)) {
            return orphans;
        }
        // les tuiles en bas sont ajoutées s'il y en a moins que 3
        while (iterator->hasNext()) {
            ITileNode* current = iterator->next();
            if (current == node) { // on saute la tuiles présente et on quitte la boucle
                break;
            }
            if (lowerCount < 3) {
                orphans->push_back(current);
            }
        }
        // les tuiles en haut sont ajoutées s'il y en a moins que 3
        while (iterator->hasNext()) {
            ITileNode* current = iterator->next();
            if (higherCount < 3) {
                orphans->push_back(current);
            }
        }
    }
    // - Les invalides -
    else {
        while (iterator->hasNext()) {
            ITileNode* current = iterator->next();
            if (current != node) {
                orphans->push_back(current);
            }
        }
    }
    delete iterator;
    return orphans;
}


bool MeldBuilder::replaceOrphanedTiles(std::vector<ITileNode*>* sequenceExtension, ITileNode* removedNode, int recursionLevel) {
    MeldType meldType = removedNode->evaluateMeldType();
    std::vector<ITileNode*>* nodesToReplace = getOrphansFromRemoval(removedNode, meldType);

    auto it = nodesToReplace->begin();
    // passer sur chaque tuiles orphelin pour les replacer ailleurs
    while (it != nodesToReplace->end()) {
        ITileNode* n = *it;
        // on saute les tuiles qui pourraient être ajoutées à la séquence
        if (sequenceExtension != nullptr) {
            if (std::find(sequenceExtension->begin(), sequenceExtension->end(), n) != sequenceExtension->end()) {
                // il pourrait peut-être regarder si on peut replacer le noeux avec organizeNodePlacement
                //  et avoir moyen d'annuler le placement de replacement de ces tuiles si on veut les utiliser
                it++;
                continue;
            }
        }
        // pour éviter de replacer une tuile dans un meld où elle est déjà
        if (recursionLevel > 1 && meldContainTile(n, removedNode)) {
            it++;
            continue;
        }
        // on regarde si on peut placer la tuile
        bool couldBePlaced = organizeNodePlacement(n, recursionLevel + 1);
        if (couldBePlaced) {
            it = nodesToReplace->erase(it);
        }
        else {
            delete sequenceExtension;
            delete nodesToReplace;
            return false;
        }
    }
    // rendu ici, nodesToReplace ne nous intéresse plus
    delete nodesToReplace;
    nodesToReplace = nullptr;
    return true;
}

void MeldBuilder::filterJokerNeighbors(std::vector<ITileNode*>* compatibleNodes, ITileNode* node, ValueDirection direction) {
    const Tile* jokerTile = node->getTileWrapper()->tile;
    if (jokerTile->value == 0 || node->getMeldTileCount() == 1) {
        return;
    }
    auto it = compatibleNodes->begin();
    // on s'assure que les tuiles connectées au joker sont bien de la bonne couleur et de la bonne valeur
    while (it != compatibleNodes->end()) {
        const Tile* compatibleTile = (*it)->getTileWrapper()->tile;
        if (direction == ValueDirection::Lower) {
            if (compatibleTile->value != jokerTile->value - 1 || compatibleTile->color != jokerTile->color) {
                it = compatibleNodes->erase(it);
            }
            else {
                it++;
            }
        }
        else if (direction == ValueDirection::Higher) {
            if (compatibleTile->value != jokerTile->value + 1 || compatibleTile->color != jokerTile->color) {
                it = compatibleNodes->erase(it);
            }
            else {
                it++;
            }
        }
        else if (direction == ValueDirection::Same) {
            if (jokerTile->value != compatibleTile->value) {
                it = compatibleNodes->erase(it);
            }
            else {
                it++;
            }
        }
    }
}

std::vector<ITileNode*>* MeldBuilder::getFilteredCompatibleNodes(std::vector<ITileNode*>& currentMeldBuild, ITileNode* node, ValueDirection direction) {
    std::vector<ITileNode*>* compatibleNodes = copyCompatibleNodes(node, direction);
    auto it = compatibleNodes->begin();
    while (it != compatibleNodes->end()) {
        ITileNode* compatibleNode = *it;
        // on enlève les tuiles déjà dans le meld
        bool isInMeldBuild = false;
        for (ITileNode* currentMeldNode : currentMeldBuild) {
            if (compatibleNode == currentMeldNode) {
                isInMeldBuild = true;
                break;
            }
        }
        if (isInMeldBuild) {
            it = compatibleNodes->erase(it);
        }
        // on ne joue pas nos jokers si _useJoker est à false
        else if (compatibleNode->getTileWrapper()->tile->isJoker && _useJoker == false && compatibleNode->getTileWrapper()->isPlayerTile) {
            it = compatibleNodes->erase(it);
        }
        else {
            it++;
        }
    }
    if (node->getTileWrapper()->tile->isJoker) {
        filterJokerNeighbors(compatibleNodes, node, direction);
    }
    return compatibleNodes;
}


void MeldBuilder::orderCompatibleNodes(std::vector<ITileNode*>* compatibleNodes, ITileNode* node) {
    bool hasPlayerNode = false;
    auto it = compatibleNodes->begin();
    while (it != compatibleNodes->end()) {
        ITileNode* compatibleNode = *it;
        // mettre les tuiles du joueur en avant
        if (compatibleNode->getTileWrapper()->isPlayerTile) {
            compatibleNodes->erase(it);
            compatibleNodes->insert(compatibleNodes->begin(), compatibleNode);
            hasPlayerNode = true;
        }
        // mettre les tuiles du meld en avant (mais après celle du joueur s'il y en a une)
        else if (meldContainTile(compatibleNode, node)) {
            int insertionIndex = 0;
            if (hasPlayerNode) {
                insertionIndex = 1;
            }
            compatibleNodes->erase(it);
            compatibleNodes->insert(compatibleNodes->begin() + insertionIndex, compatibleNode);
        }
        it++;
    }
}

bool MeldBuilder::formSet(ITileNode* nodeToPlace, std::vector<ITileNode*>& currentMeldBuild, int recursionLevel) {
    if (recursionLevel > _maxRecursionLevel) {
        return false;
    }
    ValueDirection direction = ValueDirection::Same;


    // si la tuile est dans un meld, on regarde si on peut la retirer de son meld
    if (nodeToPlace->getMeldTileCount() > 1) {
        if (replaceOrphanedTiles(nullptr, nodeToPlace, recursionLevel) == false) {
            return false;
        }
    }

    // on ajoute la tuile actuelle dans le meld en construction et on fait les liens dans le graphe
    ICommand* linkNodeCommand = new AddNodeCommand(nodeToPlace, &currentMeldBuild, direction);
    linkNodeCommand->execute();

    // point de sortie si on a asser de tuiles pour un meld
    if (currentMeldBuild.size() >= 3) {
        // on ajoute les tuiles de séquence qui vont dans le même sens s'il y en a moins que 3
        _currentGameState->add(new MeldRebuildGroup(currentMeldBuild));
        delete linkNodeCommand;
        return true;
    }

    // on continue de construire le meld en appelant cette méthode de façon récursive
    std::vector<ITileNode*>* compatibleNodes = getFilteredCompatibleNodes(currentMeldBuild, nodeToPlace, direction);
    orderCompatibleNodes(compatibleNodes, nodeToPlace);

    for (ITileNode* n : *compatibleNodes) {
        bool isSameColor = false;
        for (ITileNode* currentMeldNode : currentMeldBuild) {
            if (n->getTileWrapper()->tile->color == currentMeldNode->getTileWrapper()->tile->color) {
                isSameColor = true;
                break;
            }
        }
        // on augmente pas le niveau de récursion lorsqu'on se promène en largeur
        if (isSameColor == false) {
            if (formSet(n, currentMeldBuild, recursionLevel)) {
                delete compatibleNodes;
                delete linkNodeCommand;
                return true;
            }
        }
    }
    // on, a pas réussi à construire le meld, on remet comme avant
    linkNodeCommand->undo();
    delete linkNodeCommand;
    delete compatibleNodes;
    return false;
}

bool MeldBuilder::formSequence(ITileNode* nodeToPlace, std::vector<ITileNode*>& currentMeldBuild, std::stack<ICommand*>* linkNodeCommands,
    ValueDirection direction, int recursionLevel, bool isBuildingBothWays) {
    // Early out si on est rendu à la limite de récursion
    if (recursionLevel > _maxRecursionLevel) {
        return false;
    }

    std::vector<ITileNode*>* sequenceExtension = getSequenceExtensions(nodeToPlace, direction);
    if (nodeToPlace->getTileWrapper()->tile->isJoker) {
        sequenceExtension->clear();
    }

    // si la tuile est dans un meld, on regarde si on peut la retirer de son meld
    if (nodeToPlace->getMeldTileCount() > 1) {
        if (replaceOrphanedTiles(sequenceExtension, nodeToPlace, recursionLevel) == false) {
            return false;
        }
    }

    // TODO : Si on est en recursion > 1, on enlève la tuile compatible faisant partie du meld de nodeToPlace

    // on ajoute la tuile actuelle dans le meld en construction et on fait les liens dans le graphe
    ICommand* linkNodeCommand = new AddNodeCommand(nodeToPlace, &currentMeldBuild, direction);
    linkNodeCommands->push(linkNodeCommand);
    linkNodeCommand->execute();


    // point de sortie si on a asser de tuiles pour un meld
    if (currentMeldBuild.size() >= 3 && (sequenceExtension->size() >= 3 || sequenceExtension->size() == 0)) {
        // on ajoute les tuiles de séquence qui vont dans le même sens s'il y en a moins que 3
        _currentGameState->add(new MeldRebuildGroup(currentMeldBuild));
        delete linkNodeCommand;
        return true;
    }

    std::vector<ITileNode*>* compatibleNodes = getFilteredCompatibleNodes(currentMeldBuild, nodeToPlace, direction);
    if (compatibleNodes->size() == 0) {
        // on a pas réussi à former une séquence dans cette direction, mais on annule pas pour la continuer dans l'autre sens
        if (isBuildingBothWays == false) {
            linkNodeCommand->undo();
            linkNodeCommands->pop();
            delete linkNodeCommand;
        }
        delete compatibleNodes;
        return false;
    }
    // si on est dans une séquence existante trop petite pour former un meld, on la continue et on ne considère pas les autres tuiles
    if (sequenceExtension->size() > 0 && sequenceExtension->size() < 3) {
        compatibleNodes->clear();
        ITileNode* sequenceNode = nullptr;
        if (direction == ValueDirection::Lower) {
            sequenceNode = sequenceExtension->back();
        }
        else if (direction == ValueDirection::Higher) {
            sequenceNode = sequenceExtension->front();
        }
        compatibleNodes->insert(compatibleNodes->begin(), sequenceNode);
    }

    // on continue de construire le meld en appelant cette méthode de façon récursive
    // trier en ordre de priorité : les tuiles du joueur, les tuiles du meld et ensuite les autres tuiles
    orderCompatibleNodes(compatibleNodes, nodeToPlace);
    for (ITileNode* n : *compatibleNodes) {
        // appel résursif de cette méthode pour continuer la séquence dans cette direction
        // on n'augmente pas le niveau de récursion puisqu'il s'agit de la même séquence
        if (formSequence(n, currentMeldBuild, linkNodeCommands, direction, recursionLevel, isBuildingBothWays)) {
            delete compatibleNodes;
            delete linkNodeCommand;
            return true;
        }
        else if (isBuildingBothWays) {
            break; // pour ne pas ajouter d'autre fois des tuiles de la même valeur
        }
    }
    if (isBuildingBothWays == false) {
        linkNodeCommand->undo();
        linkNodeCommands->pop();
        delete linkNodeCommand;
    }
    // on, a pas réussi à construire le meld, on remet comme avant
    delete compatibleNodes;
    return false;
}

bool hasPlayerNode(std::vector<ITileNode*>& nodes) {
    for (ITileNode* node : nodes) {
        if (node->getTileWrapper()->isPlayerTile) {
            return true;
        }
    }
    return false;
}

std::vector<MeldType> MeldBuilder::getMeldTypeOrder(ITileNode* node) {
    std::vector<MeldType> typeOrder;
    std::vector<ITileNode*> compatibleSameNodes = node->getCompatibleNodes(ValueDirection::Same);
    if (compatibleSameNodes.size() > 0) {
        typeOrder.push_back(MeldType::SET);
    }
    std::vector<ITileNode*> compatibleLowerNodes = node->getCompatibleNodes(ValueDirection::Lower);
    std::vector<ITileNode*> compatibleHigherNodes = node->getCompatibleNodes(ValueDirection::Higher);
    if (compatibleLowerNodes.size() > 0 || compatibleHigherNodes.size() > 0) {
        if (hasPlayerNode(compatibleLowerNodes) || hasPlayerNode(compatibleHigherNodes)) {
            typeOrder.insert(typeOrder.begin(), MeldType::SEQUENCE);
        }
        else {
            typeOrder.push_back(MeldType::SEQUENCE);
        }
    }
    return typeOrder;
}

bool MeldBuilder::organizeNodePlacement(ITileNode* node, int recursionLevel) {
    if (recursionLevel > _maxRecursionLevel) {
        return false;
    }

    std::vector<MeldType> meldTypeOrder = getMeldTypeOrder(node);
    // passer sur toutes les directions
    for (MeldType type : meldTypeOrder) {
        std::vector<ITileNode*> currentMeldBuild;
        if (type == MeldType::SET) {
            if (extendSet(node)) {
                return true;
            }
            else if (formSet(node, currentMeldBuild, recursionLevel)) {
                return true;
            }
        }
        else if (type == MeldType::SEQUENCE) {
            std::stack<ICommand*> linkNodeCommands;
            if (extendSequence(node)) {
                return true;
            }
            // tenter de faire une séquence dans une des deux directions
            // en bas
            if (formSequence(node, currentMeldBuild, &linkNodeCommands, ValueDirection::Lower, recursionLevel, false)) {
                return true;
            }
            else {
                ICommand::cancelCommands(linkNodeCommands);
            }
            // en Haut
            if (formSequence(node, currentMeldBuild, &linkNodeCommands, ValueDirection::Higher, recursionLevel, false)) {
                return true;
            }
            else {
                ICommand::cancelCommands(linkNodeCommands);
            }
            // sinon, on essaie dans les 2 directfions en même temps
            // *si on est rendu ici, formSequence( lower ) a assurément échoué la première fois
            if (formSequence(node, currentMeldBuild, &linkNodeCommands, ValueDirection::Lower, recursionLevel, true) == false) {
                const std::vector<ITileNode*> compatibleNodes = node->getCompatibleNodes(ValueDirection::Higher);
                for (ITileNode* n : compatibleNodes) {
                    if (formSequence(n, currentMeldBuild, &linkNodeCommands, ValueDirection::Higher, recursionLevel, false)) {
                        return true;
                    }
                }
                ICommand::cancelCommands(linkNodeCommands);
            }
            else {
                throw new std::runtime_error("Erreur: formSequence ( lower ) a réussi la première fois");
            }

        }
    }
    return false;
}

// ------------------------------


IGameBuildState* MeldBuilder::buildGame(ITileGraph* tilGraph, bool useJoker) {
    _useJoker = useJoker;
    delete _currentGameState;
    _currentGameState = new GameBuildState();
    std::vector<ITileNode*> playerTileNodes = tilGraph->getPlayerTileNodes();
    // passer sur tous les noeuds de tuiles du joueur
    for (ITileNode* playerTileNode : playerTileNodes) {
        // si ce n'est pas déjà inclue dans le jeu en construction et on ne tente pas de jouer un joker comme ça
        if (_currentGameState->contains(playerTileNode) == false && playerTileNode->getTileWrapper()->tile->isJoker == false) {
            organizeNodePlacement(playerTileNode, 1);
        }
    }
    return _currentGameState;
}
