#include <gui/components/fps_counter.h>
#include <gui/utilities/font_loader.h>

using namespace gui;

FpsCounter::FpsCounter() {
    _font = FontLoader::loadFont("assets/fonts/Inconsolata-Regular.ttf");
    _text.setFont(*_font);
    _text.setCharacterSize(15);
    _text.setFillColor(sf::Color(255, 255, 255, 155));
    _text.setPosition(1, 1);
}

void FpsCounter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    _frameCount++;
    if (_clock.getElapsedTime().asSeconds() >= 1.f) {
        _text.setString(std::to_string(_frameCount));
        _frameCount = 0;
        _clock.restart();
    }
    target.draw(_text, states);
}