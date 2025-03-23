#pragma once
#include <string>

#include <game/tile.h>

enum class EventType {
    GIVE_TURN,
    PLAYER_ACTION,
    GAME_STATUS
};

class IEvent {
public:
    virtual const EventType getType() const = 0;
    virtual std::string toString() const = 0;
};

class GiveTurnEvent : public IEvent {
private:
    const std::string _playerName;
    const EventType _type {EventType::GIVE_TURN};
public:
    GiveTurnEvent(const std::string& playerName) : _playerName(playerName) {}
    const EventType getType() const override { return _type; }
    std::string toString() const override;
};


class DrawEvent : public IEvent {
private:
    const std::string _playerName;
    const EventType _type {EventType::PLAYER_ACTION};
public:
    DrawEvent(const std::string& playerName) : _playerName(playerName) {}
    const EventType getType() const override { return _type; }
    std::string toString() const override;
};

class PlayerPlayedEvent : public IEvent {
private:
    const std::string _playerName;
    const EventType _type {EventType::PLAYER_ACTION};
public:
    PlayerPlayedEvent(const std::string& playerName) : _playerName(playerName) {}
    const EventType getType() const override { return _type; }
    std::string toString() const override;
};

class GameOverEvent : public IEvent {
private:
    const std::string _playerName;
    const EventType _type {EventType::GAME_STATUS};
public:
    GameOverEvent(const std::string& playerName) : _playerName(playerName) {}
    const EventType getType() const override { return _type; }
    std::string toString() const override;
};

class CustomEvent : public IEvent {
private:
    const std::string _message;
    const EventType _type {EventType::GAME_STATUS};
public:
    CustomEvent(const std::string& message) : _message(message) {}
    const EventType getType() const override { return _type; }
    std::string toString() const override;
};