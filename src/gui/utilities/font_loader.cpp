#include <gui/utilities/font_loader.h>
#include <io/paths.h>

using namespace gui;

std::string FontLoader::getFontPath(Font font) {
    std::string path = paths::dataRoot + "fonts/";
    switch (font) {
        case Font::Inconsolata:
            return path + "Inconsolata-Regular.ttf";
        case Font::Inter:
            return path + "Inter_28pt-Bold.ttf";
        default:
            throw std::runtime_error("Police inconnue");
    }
}

sf::Font* FontLoader::loadFont(Font fontEnum) {
    sf::Font* font = new sf::Font();
    if (!font->loadFromFile(getFontPath(fontEnum))) {
        throw std::runtime_error("Erreur lors du chargement de la police");
    }
    return font;
}
