#include "doubleDamageAbility.h"

bool DoubleDamageAbility::use() {
    if (!targetField) return false;
    targetField->setNextAttackDoubleDamage(true);
    return true;
}

std::string DoubleDamageAbility::getName() const {
    return "Double Damage";
}