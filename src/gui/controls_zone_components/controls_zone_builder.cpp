#include <gui/controls_zone_components/controls_zone_builder.h>
#include <gui/controls_zone_components/controls_zone_component.h>
#include <game/game_dependencies.h>
#include <smart_ptr.h>

using namespace gui;

GameControlsBuilder::GameControlsBuilder(IPlayerController* controller, IWindowEventsPublisher* const eventPublisher) {
    ptr<ControlsZoneComponent> controlZoneComponent = std::make_shared<ControlsZoneComponent>(controller);
    _controlsContainer = controlZoneComponent.get();
    eventPublisher->subscribe(sf::Event::EventType::MouseButtonPressed, controlZoneComponent);
    eventPublisher->subscribe(sf::Event::EventType::MouseMoved, controlZoneComponent);
    _controlsContainer->setFillColor(sf::Color(0, 170, 40));
}

void GameControlsBuilder::setEmplacement(const sf::FloatRect& emplacement) {
    _controlsContainer->setSize(sf::Vector2f(emplacement.width, emplacement.height));
    _controlsContainer->setPosition(emplacement.left, emplacement.top);
}

sf::Drawable* GameControlsBuilder::getDrawable() {
    return _controlsContainer;
}
