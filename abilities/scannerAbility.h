#ifndef SCANNER_ABILITY_H
#define SCANNER_ABILITY_H

#include "ability.h"
#include "../mainElements/gamefield.h"
#include "../mainElements/shipmanager.h"

class ScannerAbility : public Ability {
    public:
    ScannerAbility() = default;
    ScannerAbility(GameField& field, ShipManager& manager);
    bool use() override;
    std::string getName() const override;

    private:
        GameField* targetField{nullptr};
        ShipManager* shipManager{nullptr};
        int x{-1};
        int y{-1};
};

#endif