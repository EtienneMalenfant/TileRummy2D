#include <gui/controls_zone_components/controls_zone_component.h>
#include <gui/utilities/font_loader.h>
#include <gui/utilities/generic_coords_math.h>

using namespace gui;

ControlsZoneComponent::ControlsZoneComponent(IPlayerController* controller) : _controller(controller) {
    sf::Font* font = FontLoader::loadFont("assets/fonts/Inconsolata-Regular.ttf");
    _buttons[0] = new Button("Piger", font);
    _buttons[1] = new Button("Confirmer", font);
    _buttons[2] = new Button("Annuler", font);
    for (int i = 0; i < _nbButtons; i++) {
        _buttons[i]->setFillColor(_buttonColor);
        _buttons[i]->setOutlineColor(sf::Color::Black);
        _buttons[i]->setOutlineThickness(-2);
    }
}

ControlsZoneComponent::~ControlsZoneComponent() {
    for (int i = 0; i < _nbButtons; i++) {
        delete _buttons[i];
    }
}

void ControlsZoneComponent::setPosition(float x, float y) {
    Container::setPosition(x, y);

    float buttonZoneHeight = _buttons[0]->getGlobalBounds().height * 3 + _buttonSpacing * 2;
    sf::FloatRect buttonsLocalBounds(0, 0, _buttons[0]->getGlobalBounds().width, buttonZoneHeight);
    sf::Vector2f offset = CoordsMath::getOffsetToCenter(Container::getGlobalBounds(), buttonsLocalBounds);
    for (int i = 0; i < _nbButtons; i++) {
        _buttons[i]->setPosition(x + offset.x, y + offset.y + i * (_buttons[i]->getGlobalBounds().height + _buttonSpacing));
    }
}

void ControlsZoneComponent::setPosition(const sf::Vector2f& position) {
    setPosition(position.x, position.y);
}

void ControlsZoneComponent::setSize(const sf::Vector2f& size) {
    Container::setSize(size);
    float width = (size.x / 3) * 2; // le 2 / 3 de la largeur
    float height = std::min(40, ((int)size.y / 3) - _buttonSpacing); // le 1 / 3 de la hauteur
    sf::Vector2f buttonSize(width, height);
    for (int i = 0; i < _nbButtons; i++) {
        _buttons[i]->setSize(buttonSize);
    }
    setPosition(Container::getPosition());
}

void ControlsZoneComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Container::draw(target, states);
    for (int i = 0; i < _nbButtons; i++) {
        target.draw(*_buttons[i], states);
    }
}

// ------------------------------

void ControlsZoneComponent::handle(const sf::Event& event) {
    // si c'est un clique dans sa zone
    if (event.type == sf::Event::EventType::MouseButtonPressed && Container::getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
        for (int i = 0; i < _nbButtons; i++) {
            // le clique est sur un bouton
            if (_buttons[i]->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                std::string buttonName = _buttons[i]->getName();
                if (buttonName == "Piger") {
                    _controller->draw();
                } else if (buttonName == "Confirmer") {
                    _controller->commitActions();
                } else if (buttonName == "Annuler") {
                    _controller->cancelActions();
                }
            }
        }
    }
    else if (event.type == sf::Event::EventType::MouseMoved && Container::getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
        for (int i = 0; i < _nbButtons; i++) {
            if (_buttons[i]->getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                _buttons[i]->setFillColor(sf::Color(232, 190, 104));
            } else {
                _buttons[i]->setFillColor(_buttonColor);
            }
        }
    }
}