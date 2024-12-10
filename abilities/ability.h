#ifndef ABILITY_H
#define ABILITY_H

#include <string>
class GameField;
class ShipManager;

class Ability {
    public:
    virtual ~Ability() = default;
    virtual bool use() = 0;
    virtual std::string getName() const = 0;
};

#endif