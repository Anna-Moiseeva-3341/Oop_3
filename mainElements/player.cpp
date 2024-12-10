#include "player.h"

Player::Player(GameField* targetField, ShipManager* enemyShips, AbilityManager* abilities)
    : targetField(targetField), enemyShips(enemyShips), abilities(abilities) {}

bool Player::attack(int x, int y) {
    if (!targetField || !enemyShips) {
        return false;
    }
    try {
        targetField->attackCell(x, y, *enemyShips);
        return true;
    } catch (const OutOfFieldException&) {
        return false;
    }
}

bool Player::useAbility() {
    if (!abilities || !abilities->hasAbilities()) {
        return false;
    }
    try {
        return abilities->useAbility(*targetField, *enemyShips);
    } catch (const NoAbilityException&) {
        return false;
    }
}

bool Player::hasAbilities() const {
    return abilities && abilities->hasAbilities();
}

std::string Player::getCurrentAbilityName() const {
    if (!abilities) {
        return "";
    }
    try {
        return abilities->getFirstAbilityName();
    } catch (const NoAbilityException&) {
        return "";
    }
}