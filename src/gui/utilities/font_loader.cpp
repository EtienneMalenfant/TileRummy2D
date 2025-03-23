#include <gui/utilities/font_loader.h>

using namespace gui;

sf::Font* FontLoader::loadFont(const std::string& fontPath) {
    sf::Font* font = new sf::Font();
    if (!font->loadFromFile(fontPath)) {
        throw std::runtime_error("Erreur lors du chargement de la police");
    }
    return font;
}