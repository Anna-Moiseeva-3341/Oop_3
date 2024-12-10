#ifndef PLAYER_H
#define PLAYER_H

#include "gamefield.h"
#include "shipmanager.h"
#include "../abilities/abilityManager.h"

class Player {
protected:
    GameField* targetField;
    ShipManager* enemyShips;
    AbilityManager* abilities;

public:
    Player(GameField* targetField, ShipManager* enemyShips, AbilityManager* abilities = nullptr);
    virtual ~Player() = default;
    virtual bool attack(int x, int y);
    bool useAbility();
    bool hasAbilities() const;
    std::string getCurrentAbilityName() const;
};

#endif