#pragma once
#include <SFML/Graphics.hpp>

namespace gui {

    class IWindowEventHandler {
    public:
        virtual void handle(const sf::Event& event) = 0;

        virtual ~IWindowEventHandler() = default;
    };
}