#include "gamefield.h"
#include "../abilities/abilityManager.h"
#include <stdexcept>
#include <iostream>

GameField::GameField(int width, int height)
    : width(width), height(height), validation_flag(false) {
    if (width > 0 && height > 0) {
        validation_flag = true;
        field.resize(height, std::vector<CellInfo>(width));
    }
}

void GameField::onShipDestroyed() {
    if (!validation_flag) {
        return;
    }
    if (abilityManager != nullptr) {
        abilityManager->addRandomAbility();
    }
}

bool GameField::isValid() const {
    return validation_flag;
}

GameField::GameField(const GameField& other) {
    copyField(other);
}

GameField::GameField(GameField&& other) noexcept {
    moveField(other);
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        copyField(other);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        moveField(other);
    }
    return *this;
}

GameField::~GameField() {
}

void GameField::copyField(const GameField& other) {
    width = other.width;
    height = other.height;
    field = other.field;
    validation_flag = other.validation_flag;
}

void GameField::moveField(GameField& other) {
    width = other.width;
    height = other.height;
    field = std::move(other.field);
    validation_flag = other.validation_flag;

    other.width = 0;
    other.height = 0;
    other.field.clear();
    other.validation_flag = false;
}

bool GameField::canPlaceShip(const Ship& ship, int x, int y, Orientation orientation) const {
    if (!validation_flag || !ship.isValid()) {
        return false;
    }

    int shipLength = ship.getLength();

    for (int i = 0; i < shipLength; ++i) {
        int xi = x;
        int yi = y;

        if (orientation == Orientation::Horizontal) {
            xi += i;
        } else {
            yi += i;
        }

        if (xi < 0 || xi >= width || yi < 0 || yi >= height) {
            return false;
        }

        if (field[yi][xi].status == CellStatus::Ship) {
            return false;
        }

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int xj = xi + dx;
                int yj = yi + dy;

                if (xj >= 0 && xj < width && yj >= 0 && yj < height) {
                    if (field[yj][xj].status == CellStatus::Ship) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool GameField::placeShip(Ship& ship, int x, int y, Orientation orientation) {
    if (!validation_flag || !ship.isValid()) {
        throw ShipPlacementException();
    }
    if (!canPlaceShip(ship, x, y, orientation)) {
        throw ShipPlacementException();
    }

    int shipLength = ship.getLength();
    ship.setOrientation(orientation); 
    for (int i = 0; i < shipLength; ++i) {
        int xi = x;
        int yi = y;

        if (orientation == Orientation::Horizontal) {
            xi += i;
        } else {
            yi += i;
        }

        field[yi][xi].status = CellStatus::Ship;
        field[yi][xi].shipPtr = &ship;
        field[yi][xi].segmentIndex = i;
    }

    return true;
}

CellStatus GameField::getCellStatus(int x, int y) const {
    if (!validation_flag) {
        throw OutOfFieldException();
    }
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw OutOfFieldException();
    }
    return field[y][x].status;
}

void GameField::attackCell(int x, int y, ShipManager& shipManager) {
    if (!validation_flag || !shipManager.isValid()) {
        throw OutOfFieldException();
    }
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw OutOfFieldException();
    }
    
    CellInfo& cell = field[y][x];
    bool wasDoubleDamage = nextAttackDoubleDamage; 
    nextAttackDoubleDamage = false;  
    
    if (cell.status == CellStatus::Unknown || cell.status == CellStatus::Empty) {
        cell.status = CellStatus::Empty;
        std::cout << "Miss!\n";
    } else if (cell.status == CellStatus::Ship && cell.shipPtr != nullptr) {
        int segmentIndex = cell.segmentIndex;
        int damage = wasDoubleDamage ? 2 : 1;
        
        bool wasDestroyedBefore = cell.shipPtr->isDestroyed();
        shipManager.applyDamageToShip(*cell.shipPtr, static_cast<std::size_t>(segmentIndex), damage);
        
        if (cell.shipPtr->isDestroyed() && !wasDestroyedBefore) {
            std::cout << "Ship destroyed!\n";
            onShipDestroyed();
        } else {
            std::cout << "Hit!\n";
        }
    }
}

const CellInfo& GameField::getCell(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw OutOfFieldException();
    }
    return field[y][x];
}

void GameField::setCellState(int x, int y, CellStatus status, Ship* shipPtr, int segmentIndex) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw OutOfFieldException();
    }
    field[y][x].status = status;
    field[y][x].shipPtr = shipPtr;
    field[y][x].segmentIndex = segmentIndex;
}

Ship* GameField::getShip(int x, int y) {
    if (!validation_flag) {
        return nullptr;
    }
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return nullptr;
    }
    return field[y][x].shipPtr;
}
