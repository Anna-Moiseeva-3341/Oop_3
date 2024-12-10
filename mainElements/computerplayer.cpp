#include "computerplayer.h"

ComputerPlayer::ComputerPlayer(GameField* targetField, ShipManager* enemyShips)
    : targetField(targetField), 
      enemyShips(enemyShips), 
      rng(std::random_device{}()) 
{
}

bool ComputerPlayer::makeMove() {
    if (!targetField || !enemyShips) {
        return false;
    }

    int x = std::uniform_int_distribution<>(0, targetField->getWidth() - 1)(rng);
    int y = std::uniform_int_distribution<>(0, targetField->getHeight() - 1)(rng);

    try {
        targetField->attackCell(x, y, *enemyShips);
        return true;
    } catch (const OutOfFieldException&) {
        return false;
    }
}