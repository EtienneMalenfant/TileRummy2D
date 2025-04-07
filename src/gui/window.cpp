#include <gui/window.h>

using namespace gui;

Window::Window(sf::RenderWindow* window,sf::Drawable* rootObj, IWindowEventsPublisher* eventPublisher) :
    _window(window), _app(rootObj), _eventPublisher(eventPublisher) {}

gui::Window::~Window() {
    delete _window;
}

bool gui::Window::isOpen() const {
    return _window->isOpen();
}

void gui::Window::close() {
    _window->close();
}

void Window::mapCoord(sf::Event& event) {
    // fix pour changer les coords de la fenêtre en coords de la vue (ce que mes composants utilisent)
    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
        sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f viewCoord = _window->mapPixelToCoords(mousePos);
        event.mouseButton.x = viewCoord.x;
        event.mouseButton.y = viewCoord.y;
    } else if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos(sf::Mouse::getPosition(*_window));
        sf::Vector2f viewCoord = _window->mapPixelToCoords(mousePos);

        event.mouseMove.x = viewCoord.x;
        event.mouseMove.y = viewCoord.y;
    }

}

void gui::Window::handleEvents() {
    sf::Event event;
    while (_window->pollEvent(event)) {
        mapCoord(event);
        _eventPublisher->notify(event);
    }
}

void gui::Window::clear() {
    _window->clear(sf::Color(0, 60, 0));
}

void gui::Window::draw() {
    _window->draw(*_app);
    _window->display();
}
