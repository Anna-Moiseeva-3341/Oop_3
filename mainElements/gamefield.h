#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include "ship.h"
#include "shipmanager.h"
#include <map>
#include "../exceptions/gameExceptions.h"

class AbilityManager;

enum class CellStatus {
    Unknown,
    Empty,
    Ship
};

struct CellInfo {
    CellStatus status;
    Ship* shipPtr;
    int segmentIndex;
    CellInfo() : status(CellStatus::Unknown), shipPtr(nullptr), segmentIndex(-1) {}
};

class GameField {
public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    ~GameField();

    bool isValid() const;
    bool placeShip(Ship& ship, int x, int y, Orientation orientation);
    CellStatus getCellStatus(int x, int y) const;
    void attackCell(int x, int y, ShipManager& shipManager);
    Ship* getShip(int x, int y); 

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    const CellInfo& getCell(int x, int y) const;
    void setCellState(int x, int y, CellStatus status, Ship* shipPtr, int segmentIndex);

    bool canPlaceShip(const Ship& ship, int x, int y, Orientation orientation) const;

    void setAbilityManager(AbilityManager* manager) {
        if (validation_flag)
            abilityManager = manager;
    }
    void setNextAttackDoubleDamage(bool value) {
        nextAttackDoubleDamage = value;
    }
    bool isNextAttackDoubleDamage() const {
        return nextAttackDoubleDamage;  
    }

private:
    int width;
    int height;
    std::vector<std::vector<CellInfo>> field;
    std::vector<Ship*> ships; 
    bool validation_flag;
    AbilityManager* abilityManager;
    void copyField(const GameField& other);
    void moveField(GameField& other);
    bool nextAttackDoubleDamage{false}; 
    void onShipDestroyed();
};

#endif