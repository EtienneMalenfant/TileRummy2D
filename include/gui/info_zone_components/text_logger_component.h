#pragma once
#include <SFML/Graphics.hpp>
#include <boost/circular_buffer.hpp>
#include <gui/components/container.h>
#include <logger.h>
#include <string>

namespace gui {

    class TextLoggerComponent : public Container, public ILogger {
    private:
        boost::circular_buffer<sf::Text*> _logs;
        
        sf::Font* _font;
        u_int _capacity = 10;
        const u_int _fontSize = 16;
        const u_int _ySpacing = 10;
        const u_int _margin = 10;
        void updateLogsPosition();

        sf::Text* getTextLog(const std::string& message);
        void addLog(sf::Text* log);
    public:
        TextLoggerComponent(sf::Font* font);
        void setSize(const sf::Vector2f& size) override;
        void setPosition(float x, float y) override;
        void setPosition(const sf::Vector2f& position) override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void log(const std::string& message) override;
        void log(const std::string& message, LogType logType) override;
    };
}