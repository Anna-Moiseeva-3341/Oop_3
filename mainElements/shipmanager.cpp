#include "shipmanager.h"
#include <sstream>
#include <cstddef>

ShipManager::ShipManager(const std::vector<int>& shipSizes) : validation_flag(true) {
    for (int size : shipSizes) {
        Ship ship(size, Orientation::Horizontal);
        if (!ship.isValid()) {
            validation_flag = false;
            break;
        }
        ships.push_back(ship);
    }
}

bool ShipManager::isValid() const {
    return validation_flag;
}

const std::vector<Ship>& ShipManager::getShips() const {
    return ships;
}

std::vector<Ship>& ShipManager::getShips() {
    return ships;
}

Ship& ShipManager::getShip(std::size_t index) {
    if (!validation_flag) {
        throw std::logic_error("Invalid ShipManager object.");
    }
    if (index >= ships.size()) {
        throw std::out_of_range("Ship index out of range.");
    }
    return ships[index];
}

void ShipManager::applyDamageToShip(Ship& ship, std::size_t segmentIndex, int damage) {
    if (!validation_flag) {
        throw std::logic_error("Invalid ShipManager object.");
    }
    if (!ship.isValid()) {
        throw std::logic_error("Invalid Ship object.");
    }
    ship.applyDamage(static_cast<int>(segmentIndex), damage);
}

bool ShipManager::allShipsDestroyed() const {
    if (!validation_flag) {
        return false; 
    }
    for (const auto& ship : ships) {
        if (!ship.isDestroyed()) {
            return false;
        }
    }
    return true;
}

std::string ShipManager::getShipsStatus() const {
    if (!validation_flag) {
        return "Invalid ShipManager";
    }
    std::ostringstream status;
    status << "Ships status:\n";
    for (std::size_t i = 0; i < ships.size(); ++i) {
        status << "Ship " << i + 1 << ": " << ships[i].getStatus() << "\n";
    }
    return status.str();
}
