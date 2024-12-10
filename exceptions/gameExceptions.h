#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class NoAbilityException : public GameException {
    public:
    NoAbilityException();
};

class ShipPlacementException : public GameException {
    public:
    ShipPlacementException();
};

class OutOfFieldException : public GameException {
    public:
    OutOfFieldException();
};

#endif