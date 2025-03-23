#include <console_logger.h>
#include <iostream>

void ConsoleGameEventListener::update(ptr<IEvent> event) {
    if (event->getType() == EventType::PLAYER_ACTION || event->getType() == EventType::GAME_STATUS) {
        std::cout << event->toString() << std::endl;
    }
}