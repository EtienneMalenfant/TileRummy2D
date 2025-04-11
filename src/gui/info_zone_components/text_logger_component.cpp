#include <gui/info_zone_components/text_logger_component.h>

#include <locale>
#include <codecvt>

using namespace gui;

TextLoggerComponent::TextLoggerComponent(sf::Font* font) :
    _font(font)
{
    Container::setFillColor(sf::Color::Transparent);
    _logs.set_capacity(_capacity);
}

TextLoggerComponent::~TextLoggerComponent() {
    for (sf::Text* text : _logs) {
        delete text;
    }
}

void TextLoggerComponent::updateLogsPosition() {
    sf::FloatRect bounds = Container::getGlobalBounds();
    float x = bounds.left + _margin;
    float y = bounds.top + _margin;
    for (sf::Text* text : _logs) {
        if (text == nullptr) {
            continue;
        }
        text->setPosition(x, y);
        y += _fontSize + _ySpacing;
    }
}

void TextLoggerComponent::setSize(const sf::Vector2f& size) {
    Container::setSize(size);
    _capacity = (size.y - _margin) / ( _fontSize + _ySpacing);
    _logs.set_capacity(_capacity);
    updateLogsPosition();
}

void TextLoggerComponent::setPosition(float x, float y) {
    Container::setPosition(x, y);
    updateLogsPosition();
}

void TextLoggerComponent::setPosition(const sf::Vector2f& position) {
    Container::setPosition(position);
    updateLogsPosition();
}

void TextLoggerComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (sf::Text* text : _logs) {
        if (text == nullptr) {
            continue;
        }
        target.draw(*text, states);
    }
}

// ------------------------------

sf::Text* TextLoggerComponent::getTextLog(const std::string& message) {
    // Convertir le message en wstring pour que SFML affiche les accents
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideMessage = static_cast<std::wstring>(converter.from_bytes(message));

    return new sf::Text(wideMessage, *_font, _fontSize);
}

void TextLoggerComponent::addLog(sf::Text* log) {
    // Pour delete l'object qui va se faire remplacer
    if (_logs.full()) {
        delete _logs.front();
    }
    _logs.push_back(log);

    updateLogsPosition();
}

void TextLoggerComponent::log(const std::string& message) {
    sf::Text* log = getTextLog(message);
    log->setFillColor(sf::Color::White);
    addLog(log);
}

void TextLoggerComponent::log(const std::string& message, LogType logType) {
    sf::Text* log = getTextLog(message);
    if (logType == LogType::Important) {
        log->setFillColor(sf::Color::Green);
    }
    else if (logType == LogType::Warning) {
        log->setFillColor(sf::Color::Yellow);
    }
    else if (logType == LogType::Error) {
        log->setFillColor(sf::Color::Red);
    }
    else {
        log->setFillColor(sf::Color::White);
    }
    addLog(log);
}
