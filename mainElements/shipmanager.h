#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include <vector>
#include <string>
#include <cstddef>
#include "ship.h"

class ShipManager {
public:
    ShipManager(const std::vector<int>& shipSizes);

    bool isValid() const;
    const std::vector<Ship>& getShips() const;
    std::vector<Ship>& getShips();
    Ship& getShip(std::size_t index); 
    void applyDamageToShip(Ship& ship, std::size_t segmentIndex, int damage);
    bool allShipsDestroyed() const;
    std::string getShipsStatus() const; 

private:
    std::vector<Ship> ships;
    bool validation_flag;
};

#endif