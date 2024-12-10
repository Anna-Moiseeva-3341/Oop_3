#ifndef BOMBARD_ABILITY_H
#define BOMBARD_ABILITY_H

#include "ability.h"
#include "../mainElements/gamefield.h"
#include "../mainElements/shipmanager.h"

class BombardAbility : public Ability {
    public:
    BombardAbility() = default;
    BombardAbility(GameField& field, ShipManager& manager);
    bool use() override;
    std::string getName() const override;

    private:
        GameField* targetField{nullptr};
        ShipManager* shipManager{nullptr};
        int shipIndex{-1};
        int segmentIndex{-1};
};

#endif