#include <gui/components/app_root_component.h>

using namespace gui;

AppRootComponent::AppRootComponent(std::vector<sf::Drawable*>* components) : _components(components) {}

AppRootComponent::~AppRootComponent() {
    delete _components;
}

void AppRootComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (sf::Drawable* component : *_components) {
        target.draw(*component, states);
    }
}
