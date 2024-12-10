#include "scannerAbility.h"
#include <iostream>

ScannerAbility::ScannerAbility(GameField& field, ShipManager& manager) {
    targetField = &field;
    shipManager = &manager;
}

bool ScannerAbility::use() {
    std::cout << "Enter the X coordinate: ";
    std::cin >> x;
    std::cout << "Enter the Y coordinate: ";
    std::cin >> y;
    
    if (!targetField || !shipManager) return true;
    
    bool shipFound = false;
    
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (x + i < targetField->getWidth() && y + j < targetField->getHeight()) {
                if (targetField->getCellStatus(x + i, y + j) == CellStatus::Ship) {
                    shipFound = true;
                    std::cout << "Ship found at (" << x + i << "," << y + j << ")\n";
                }
            }
        }
    }
    
    if (!shipFound) {
        std::cout << "No ships found in the scanned area.\n";
    }
    
    return true;
} 

std::string ScannerAbility::getName() const {
    return "Scanner";
}