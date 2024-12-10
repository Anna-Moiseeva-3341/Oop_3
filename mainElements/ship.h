#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <string>

enum class SegmentState {
    Intact,
    Damaged,
    Destroyed
};

enum class Orientation {
    Horizontal,
    Vertical
};

class Ship {
public:
    Ship(int length, Orientation orientation);
    bool isValid() const; 
    int getLength() const;
    Orientation getOrientation() const;
    void setOrientation(Orientation newOrientation); 
    SegmentState getSegmentState(int index) const;
    void applyDamage(int index, int damage);
    bool isDestroyed() const;
    std::string getStatus() const;

private:
    int length;
    Orientation orientation;
    std::vector<int> segmentDamage; 
    bool validation_flag; 
};

#endif