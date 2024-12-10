#ifndef ABILITY_MANAGER_H
#define ABILITY_MANAGER_H

#include <memory>
#include <deque>   
#include "ability.h"
#include "doubleDamageAbility.h"
#include "scannerAbility.h"
#include "bombardAbility.h"
#include "../exceptions/gameExceptions.h"

class AbilityManager {
    private:
    std::deque<std::unique_ptr<Ability>> abilities; 

    public:
    const std::deque<std::unique_ptr<Ability>>& getAbilities() const { return abilities; }
    void clearAbilities() {
        abilities.clear();
    }

    void addAbility(const std::string& name) {
        if (name == "Double Damage") {
            abilities.push_back(std::make_unique<DoubleDamageAbility>());
        } else if (name == "Scanner") {
            abilities.push_back(std::make_unique<ScannerAbility>());
        } else if (name == "Bombard") {
            abilities.push_back(std::make_unique<BombardAbility>());
        }
    }
    AbilityManager();
    void addRandomAbility();
    bool useAbility(GameField& targetField, ShipManager& shipManager);
    bool hasAbilities() const;
    std::string getFirstAbilityName() const;
};

#endif