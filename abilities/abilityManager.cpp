#include "abilityManager.h"
#include <random>
#include <iostream>
#include <memory>

AbilityManager::AbilityManager() {
    addRandomAbility();
}

void AbilityManager::addRandomAbility() {
    std::random_device rd;
    std::mt19937 gen(rd());
    int abilityType = std::uniform_int_distribution<>(0, 2)(gen);
    switch(abilityType) {
        case 0: abilities.push_back(std::make_unique<DoubleDamageAbility>()); break;
        case 1: abilities.push_back(std::make_unique<ScannerAbility>()); break;
        case 2: abilities.push_back(std::make_unique<BombardAbility>()); break;
    }
}

bool AbilityManager::useAbility(GameField& targetField, ShipManager& shipManager) {
    if (abilities.empty())
        throw NoAbilityException();

    std::string abilityName = abilities.front()->getName();
    abilities.pop_front();

    if (abilityName == "Double Damage") {
        DoubleDamageAbility ability(targetField);
        return ability.use();
    }
    else if (abilityName == "Scanner") {
        ScannerAbility ability(targetField, shipManager);
        return ability.use();
    }
    else if (abilityName == "Bombard") {
        BombardAbility ability(targetField, shipManager);
        return ability.use();
    }
    return false;
}

bool AbilityManager::hasAbilities() const {
    return !abilities.empty();
}

std::string AbilityManager::getFirstAbilityName() const {
    if (abilities.empty()) {
        throw NoAbilityException();
    }
    return abilities.front()->getName();
}
