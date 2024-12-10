#include "ship.h"
#include <sstream>
#include <stdexcept>

Ship::Ship(int length, Orientation orientation) 
    : length(length), orientation(orientation), validation_flag(false) 
{
    if (length >= 1 && length <= 4) {
        validation_flag = true;
        segmentDamage.resize(length, 0); 
    }
}

bool Ship::isValid() const {
    return validation_flag;
}

int Ship::getLength() const {
    return length;
}

Orientation Ship::getOrientation() const {
    return orientation;
}

void Ship::setOrientation(Orientation newOrientation) {
    orientation = newOrientation;
}

SegmentState Ship::getSegmentState(int index) const {
    if (!validation_flag) {
        throw std::logic_error("Invalid Ship object.");
    }
    if (index < 0 || index >= length) {
        throw std::out_of_range("Segment index out of range.");
    }
    int damage = segmentDamage[index];
    if (damage == 0) {
        return SegmentState::Intact;
    } else if (damage == 1) {
        return SegmentState::Damaged;
    } else {
        return SegmentState::Destroyed;
    }
}

void Ship::applyDamage(int index, int damage) {
    if (!validation_flag) {
        throw std::logic_error("Invalid Ship object.");
    }
    if (index < 0 || index >= length) {
        throw std::out_of_range("Segment index out of range.");
    }
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative.");
    }
    segmentDamage[index] += damage;
    if (segmentDamage[index] > 2) {
        segmentDamage[index] = 2; 
    }
}

bool Ship::isDestroyed() const {
    if (!validation_flag) {
        return false; 
    }
    for (const auto& dmg : segmentDamage) {
        if (dmg < 2) {
            return false; 
        }
    }
    return true;
}

std::string Ship::getStatus() const {
    if (!validation_flag) {
        return "Invalid Ship";
    }
    std::ostringstream status;
    status << "Ship (Length: " << length << ", Orientation: " 
           << (orientation == Orientation::Horizontal ? "Horizontal" : "Vertical") 
           << "): ";
    for (int i = 0; i < length; ++i) {
        switch (getSegmentState(i)) {
            case SegmentState::Intact: status << "O"; break;
            case SegmentState::Damaged: status << "X"; break;
            case SegmentState::Destroyed: status << "#"; break;
        }
    }
    return status.str();
}
