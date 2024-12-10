#include "bombardAbility.h"
#include <random>

BombardAbility::BombardAbility(GameField& field, ShipManager& manager) {
    targetField = &field;
    shipManager = &manager;
}

bool BombardAbility::use() {
    if (!targetField || !shipManager) return false;
    
    const auto& ships = shipManager->getShips();
    if (ships.empty()) return false;

    std::vector<int> availableShips;
    for (size_t i = 0; i < ships.size(); i++) {
        if (!ships[i].isDestroyed()) {
            availableShips.push_back(i);
        }
    }

    if (availableShips.empty()) return false;
    std::random_device rd;
    std::mt19937 gen(rd());
    int randomIndex = std::uniform_int_distribution<>(0, availableShips.size()-1)(gen);
    
    shipIndex = availableShips[randomIndex];
    Ship& ship = shipManager->getShip(shipIndex);
    segmentIndex = std::uniform_int_distribution<>(0, ship.getLength()-1)(gen);
    shipManager->applyDamageToShip(ship, segmentIndex, 1);
    return true;
}

std::string BombardAbility::getName() const {
    return "Bombard";
}