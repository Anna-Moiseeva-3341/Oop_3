#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "gamefield.h"
#include "shipmanager.h"
#include <random>

class ComputerPlayer {
private:
    GameField* targetField;
    ShipManager* enemyShips;
    std::mt19937 rng;

public:
    ComputerPlayer(GameField* targetField, ShipManager* enemyShips);
    bool makeMove();
};

#endif