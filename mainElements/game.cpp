#include "game.h"
#include <fstream>

#include <iostream>

Game::Game(int width, int height)
    : state(GameStatus::NotStarted),
      currentRound(0),
      fieldWidth(width),
      fieldHeight(height),
      shipSizes{4, 3, 3, 2, 2, 2, 1, 1, 1, 1} {
          
    playerField = std::make_unique<GameField>(fieldWidth, fieldHeight);
    computerField = std::make_unique<GameField>(fieldWidth, fieldHeight);
    playerShips = std::make_unique<ShipManager>(shipSizes);
    computerShips = std::make_unique<ShipManager>(shipSizes);
    playerAbilities = std::make_unique<AbilityManager>();
    
    player = std::make_unique<Player>(computerField.get(), computerShips.get(), playerAbilities.get());
    computer = std::make_unique<ComputerPlayer>(playerField.get(), playerShips.get());
    computerField->setAbilityManager(playerAbilities.get());
}

void Game::startNewGame() {
    currentRound = 1;
    initializeNewRound(false);
    state = GameStatus::InProgress;
}

void Game::startNextRound() {
    if (state != GameStatus::PlayerWon) {
        return;
    }
    
    std::cout << "\nPreparing for round " << (currentRound + 1) << "...\n";
    
    auto savedPlayerField = std::move(playerField);
    auto savedPlayerShips = std::move(playerShips);
    auto savedAbilities = std::move(playerAbilities);
    
    currentRound++;
    
    computerField = std::make_unique<GameField>(fieldWidth, fieldHeight);
    computerShips = std::make_unique<ShipManager>(shipSizes);
    
    playerField = std::move(savedPlayerField);
    playerShips = std::move(savedPlayerShips);
    playerAbilities = std::move(savedAbilities);
    
    computerField->setAbilityManager(playerAbilities.get());
    
    player = std::make_unique<Player>(computerField.get(), computerShips.get(), playerAbilities.get());
    computer = std::make_unique<ComputerPlayer>(playerField.get(), playerShips.get());
    
    int attempts = 0;
    const int MAX_PLACEMENT_ATTEMPTS = 100;
    
    while (!placeAllShipsRandomly(*computerField, *computerShips)) {
        attempts++;
        if (attempts >= MAX_PLACEMENT_ATTEMPTS) {
            throw std::runtime_error("Failed to place computer ships after maximum attempts");
        }
        computerField = std::make_unique<GameField>(fieldWidth, fieldHeight);
        computerShips = std::make_unique<ShipManager>(shipSizes);
    }
    
    state = GameStatus::InProgress;
}

void Game::initializeNewRound(bool keepPlayerState) {
    try {
        if (!keepPlayerState) {
            playerField = std::make_unique<GameField>(fieldWidth, fieldHeight);
            playerShips = std::make_unique<ShipManager>(shipSizes);
            playerAbilities = std::make_unique<AbilityManager>();
            
            std::cout << "\nPlace your ships!\n";
            for (size_t i = 0; i < playerShips->getShips().size(); ++i) {
                Ship& ship = playerShips->getShip(i);
                bool placed = false;
                
                std::cout << "\nCurrent field state:\n";
                displayField(*this, true);
                
                while (!placed) {
                    std::cout << "\nPlacing ship of length " << ship.getLength() << "\n";
                    std::cout << "Enter coordinates (x y) and orientation (h/v): ";
                    int x, y;
                    char orientation;
                    
                    if (std::cin >> x >> y >> orientation) {
                        Orientation orient = (orientation == 'h' || orientation == 'H') ? 
                            Orientation::Horizontal : Orientation::Vertical;
                            
                        try {
                            if (x < 0 || x >= fieldWidth || y < 0 || y >= fieldHeight) {
                                std::cout << "Coordinates out of bounds. Try again.\n";
                                continue;
                            }
                            
                            if (!playerField->canPlaceShip(ship, x, y, orient)) {
                                std::cout << "Cannot place ship there. Try again.\n";
                                continue;
                            }
                            
                            placed = playerField->placeShip(ship, x, y, orient);
                            if (!placed) {
                                std::cout << "Failed to place ship. Try again.\n";
                            }
                        } catch (const ShipPlacementException&) {
                            std::cout << "Cannot place ship there. Try again.\n";
                        } catch (const std::exception& e) {
                            std::cout << "Error: " << e.what() << "\nTry again.\n";
                        }
                    } else {
                        std::cout << "Invalid input. Please enter numbers for coordinates and 'h' or 'v' for orientation.\n";
                        std::cin.clear();
                        std::cin.ignore(10000, '\n');
                    }
                }
            }
            std::cout << "\nAll ships placed successfully!\n";
        }

        computerField = std::make_unique<GameField>(fieldWidth, fieldHeight);
        computerShips = std::make_unique<ShipManager>(shipSizes);
        computerField->setAbilityManager(playerAbilities.get());
        player = std::make_unique<Player>(computerField.get(), computerShips.get(), playerAbilities.get());
        computer = std::make_unique<ComputerPlayer>(playerField.get(), playerShips.get());
        if (!placeAllShipsRandomly(*computerField, *computerShips)) {
            throw std::runtime_error("Failed to place computer ships");
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error during game initialization: " << e.what() << std::endl;
        throw;
    }
}

void Game::transferPlayerState(GameField& oldField, AbilityManager& oldAbilities,
                             GameField& newField, AbilityManager& newAbilities) {
    newField.setAbilityManager(&newAbilities);
    playerAbilities.reset(&oldAbilities);
    if (oldField.isNextAttackDoubleDamage()) {
        newField.setNextAttackDoubleDamage(true);
    }
}

bool Game::placeShipRandomly(GameField& field, Ship& ship) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    const int MAX_ATTEMPTS = 100;
    for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
        Orientation orientation = 
            (std::uniform_int_distribution<>(0, 1)(gen) == 0) ? 
            Orientation::Horizontal : Orientation::Vertical;
        
        ship.setOrientation(orientation);
        int maxX = fieldWidth - (orientation == Orientation::Horizontal ? ship.getLength() : 0);
        int maxY = fieldHeight - (orientation == Orientation::Vertical ? ship.getLength() : 0);
        
        if (maxX <= 0 || maxY <= 0) continue;
        
        int x = std::uniform_int_distribution<>(0, maxX - 1)(gen);
        int y = std::uniform_int_distribution<>(0, maxY - 1)(gen);
        
        try {
            if (field.canPlaceShip(ship, x, y, orientation)) {
                return field.placeShip(ship, x, y, orientation);
            }
        } catch (const ShipPlacementException&) {
            continue; 
        }
    }
    return false;
}

bool Game::placeAllShipsRandomly(GameField& field, ShipManager& manager) {
    const int MAX_ATTEMPTS = 100;
    
    for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
        field = GameField(fieldWidth, fieldHeight);
        
        bool allShipsPlaced = true;
        for (size_t i = 0; i < manager.getShips().size(); ++i) {
            if (!placeShipRandomly(field, manager.getShip(i))) {
                allShipsPlaced = false;
                break;
            }
        }
        
        if (allShipsPlaced) {
            return true;
        }
    }
    return false;
}

bool Game::usePlayerAbility() {
    if (state != GameStatus::InProgress) {
        return false;
    }
    return player->useAbility();
}

bool Game::makePlayerAttack(int x, int y) {
    if (state != GameStatus::InProgress) {
        return false;
    }
    
    bool success = player->attack(x, y);
        if (success && computerShips->allShipsDestroyed()) {
        state = GameStatus::PlayerWon;
    }
    
    return success;
}

bool Game::makeComputerMove() {
    if (state != GameStatus::InProgress) {
        return false;
    }
    
    bool success = computer->makeMove();
    
    if (success && playerShips->allShipsDestroyed()) {
        state = GameStatus::PlayerLost;
    }
    
    return success;
}

GameStatus Game::getGameStatus() const {
    return state;
}

int Game::getCurrentRound() const {
    return currentRound;
}

bool Game::hasPlayerAbility() const {
    return player && player->hasAbilities();
}

std::string Game::getCurrentPlayerAbilityName() const {
    return player ? player->getCurrentAbilityName() : "";
}

CellStatus Game::getPlayerFieldStatus(int x, int y) const {
    return playerField ? playerField->getCellStatus(x, y) : CellStatus::Unknown;
}

CellStatus Game::getComputerFieldStatus(int x, int y) const {
    return computerField ? computerField->getCellStatus(x, y) : CellStatus::Unknown;
}

bool Game::isGameOver() const {
    return state == GameStatus::PlayerWon || state == GameStatus::PlayerLost;
}

void Game::displayField(const Game& game, bool isPlayerField) const {
    std::cout << "  ";
    for (int x = 0; x < fieldWidth; ++x) {
        std::cout << std::setw(2) << x;
    }
    std::cout << "\n";

    for (int y = 0; y < fieldHeight; ++y) {
        std::cout << std::setw(2) << y;
        for (int x = 0; x < fieldWidth; ++x) {
            char symbol = '.';  
            CellStatus status = isPlayerField ? 
                playerField->getCellStatus(x, y) : 
                computerField->getCellStatus(x, y);
            
            Ship* ship = isPlayerField ? 
                playerField->getShip(x, y) : 
                computerField->getShip(x, y);
                
            if (ship != nullptr) {
                int segmentIndex = isPlayerField ?
                    playerField->getCell(x, y).segmentIndex :
                    computerField->getCell(x, y).segmentIndex;
                
                SegmentState segmentState = ship->getSegmentState(segmentIndex);
                
                switch (segmentState) {
                    case SegmentState::Intact:
                        symbol = isPlayerField ? 'S' : '.';  
                        break;
                    case SegmentState::Damaged:
                        symbol = 'X'; 
                        break;
                    case SegmentState::Destroyed:
                        symbol = '#'; 
                        break;
                }
            } else if (status == CellStatus::Empty) {
                symbol = 'O';  
            }
            
            std::cout << std::setw(2) << symbol;
        }
        std::cout << "\n";
    }
}

void Game::updateGameComponents(
    std::unique_ptr<GameField> pField,
    std::unique_ptr<GameField> cField,
    std::unique_ptr<ShipManager> pShips,
    std::unique_ptr<ShipManager> cShips,
    std::unique_ptr<AbilityManager> pAbilities
) {
    playerField = std::move(pField);
    computerField = std::move(cField);
    playerShips = std::move(pShips);
    computerShips = std::move(cShips);
    playerAbilities = std::move(pAbilities);
    
    player = std::make_unique<Player>(computerField.get(), computerShips.get(), playerAbilities.get());
    computer = std::make_unique<ComputerPlayer>(playerField.get(), playerShips.get());
    computerField->setAbilityManager(playerAbilities.get());
}

void Game::displayLegend() const {
    std::cout << "\nLegend:\n";
    std::cout << "  .  - Unknown cell\n";
    std::cout << "  O  - Miss\n";
    std::cout << "  S  - Your ship (only on your field)\n";
    std::cout << "  X  - Hit\n";
    std::cout << "  #  - Destroyed segment\n";
}
