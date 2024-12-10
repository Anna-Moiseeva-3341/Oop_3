#ifndef DOUBLE_DAMAGE_ABILITY_H
#define DOUBLE_DAMAGE_ABILITY_H

#include "ability.h"
#include "../mainElements/gamefield.h"
#include "../mainElements/shipmanager.h"

class DoubleDamageAbility : public Ability {
public:
    DoubleDamageAbility() = default;
    DoubleDamageAbility(GameField& field) : targetField(&field) {}
    bool use() override;
    std::string getName() const override;

private:
    GameField* targetField{nullptr};
};

#endif