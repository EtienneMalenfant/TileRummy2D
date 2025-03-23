#include <game/event.h>
using namespace std;


string GiveTurnEvent::toString() const {
    return _playerName;
}

string DrawEvent::toString() const {
    return _playerName + " a pigé une tuile";
}

string PlayerPlayedEvent::toString() const {
    return _playerName + " joue une ou des tuiles";
}

string GameOverEvent::toString() const {
    return _playerName + " a gagné la partie.";
}

string CustomEvent::toString() const {
    return _message;
}