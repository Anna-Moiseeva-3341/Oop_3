#include "gamestate.h"
#include <fstream>
#include <stdexcept>

void GameState::saveGame(const Game& game, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) {
        throw std::runtime_error("Failed to open file for saving: " + filename);
    }
    
    GameState state(const_cast<Game*>(&game));
    ofs << state;
}

void GameState::loadGame(Game& game, const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        throw std::runtime_error("Failed to open file for loading: " + filename);
    }
    
    GameState state(&game);
    ifs >> state;
}

void GameState::saveShipManager(std::ostream& os, const ShipManager& manager) const {
    const auto& ships = manager.getShips();
    os << ships.size() << '\n';
    for (const auto& ship : ships) {
        os << ship.getLength() << ' '
           << static_cast<int>(ship.getOrientation()) << ' ';
        for (int i = 0; i < ship.getLength(); ++i) {
            os << static_cast<int>(ship.getSegmentState(i)) << ' ';
        }
        os << '\n';
    }
}

void GameState::loadShipManager(std::istream& is, ShipManager& manager) {
    size_t shipCount;
    is >> shipCount;
    auto& ships = manager.getShips();
    ships.clear();
    for (size_t i = 0; i < shipCount; ++i) {
        int length, orientationInt;
        is >> length >> orientationInt;
        Ship ship(length, static_cast<Orientation>(orientationInt));
        for (int j = 0; j < length; ++j) {
            int stateInt;
            is >> stateInt;
            switch (static_cast<SegmentState>(stateInt)) {
                case SegmentState::Damaged:
                    ship.applyDamage(j, 1);
                    break;
                case SegmentState::Destroyed:
                    ship.applyDamage(j, 2);
                    break;
                default:
                    break;
            }
        }
        ships.push_back(ship);
    }
}

void GameState::saveGameField(std::ostream& os, const GameField& field, 
                            const std::vector<Ship>& ships) const {
    auto shipPtrToIndex = createShipPtrToIndexMap(ships);
    
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            const auto& cell = field.getCell(x, y);
            os << static_cast<int>(cell.status) << ' ';
            
            int shipIndex = -1;
            if (cell.shipPtr != nullptr) {
                auto it = shipPtrToIndex.find(cell.shipPtr);
                shipIndex = (it != shipPtrToIndex.end()) ? it->second : -1;
            }
            os << shipIndex << ' ' << cell.segmentIndex << ' ';
        }
        os << '\n';
    }
    os << field.isNextAttackDoubleDamage() << '\n';
}

void GameState::loadGameField(std::istream& is, GameField& field,
                            std::vector<Ship>& ships) {
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            int statusInt, shipIndex, segmentIndex;
            is >> statusInt >> shipIndex >> segmentIndex;
            
            Ship* shipPtr = (shipIndex >= 0 && static_cast<size_t>(shipIndex) < ships.size()) 
                ? &ships[shipIndex] 
                : nullptr;
                
            field.setCellState(x, y, 
                static_cast<CellStatus>(statusInt), 
                shipPtr, 
                segmentIndex);
        }
    }
    
    int nextAttackDoubleDamageInt;
    is >> nextAttackDoubleDamageInt;
    field.setNextAttackDoubleDamage(nextAttackDoubleDamageInt != 0);
}

void GameState::saveAbilityManager(std::ostream& os, const AbilityManager& manager) const {
    const auto& abilities = manager.getAbilities();
    os << abilities.size() << '\n';
    for (const auto& ability : abilities) {
        if (ability) {
            os << ability->getName() << '\n';
        } else {
            os << "Unknown\n";
        }
    }
}

void GameState::loadAbilityManager(std::istream& is, AbilityManager& manager) {
    size_t abilityCount;
    is >> abilityCount;
    
    std::string dummy;
    std::getline(is, dummy);
    
    manager.clearAbilities();
    
    for (size_t i = 0; i < abilityCount; ++i) {
        std::string abilityName;
        std::getline(is, abilityName);
        
        if (abilityName != "Double Damage" && 
            abilityName != "Scanner" && 
            abilityName != "Bombard") {
            throw std::runtime_error("Invalid ability name in save file: " + abilityName);
        }
        
        manager.addAbility(abilityName);
    }
}

std::map<const Ship*, int> GameState::createShipPtrToIndexMap(const std::vector<Ship>& ships) const {
    std::map<const Ship*, int> shipPtrToIndex;
    for (size_t i = 0; i < ships.size(); ++i) {
        shipPtrToIndex[&ships[i]] = static_cast<int>(i);
    }
    return shipPtrToIndex;
}

std::ostream& operator<<(std::ostream& os, const GameState& state) {
    if (!state.game) {
        throw std::runtime_error("No game associated with GameState");
    }

    os << static_cast<int>(state.game->getGameStatus()) << ' '
       << state.game->getCurrentRound() << ' '
       << state.game->getFieldWidth() << ' '
       << state.game->getFieldHeight() << '\n';

    const auto& shipSizes = state.game->getShipSizes();
    os << shipSizes.size();
    for (const auto& size : shipSizes) {
        os << ' ' << size;
    }
    os << '\n';

    state.saveShipManager(os, *state.game->getPlayerShips());
    state.saveShipManager(os, *state.game->getComputerShips());
    state.saveGameField(os, *state.game->getPlayerField(), state.game->getPlayerShips()->getShips());
    state.saveGameField(os, *state.game->getComputerField(), state.game->getComputerShips()->getShips());
    state.saveAbilityManager(os, *state.game->getPlayerAbilities());

    return os;
}

std::istream& operator>>(std::istream& is, GameState& state) {
    if (!state.game) {
        throw std::runtime_error("No game associated with GameState");
    }

    int stateInt, currentRound, fieldWidth, fieldHeight;
    is >> stateInt >> currentRound >> fieldWidth >> fieldHeight;

    if (fieldWidth <= 0 || fieldWidth > 10 || fieldHeight <= 0 || fieldHeight > 10) {
        throw std::runtime_error("Invalid field size in save file. Valid size is 10x10.");
    }

    size_t shipSizesCount;
    is >> shipSizesCount;
    std::vector<int> shipSizes(shipSizesCount);
    for (size_t i = 0; i < shipSizesCount; ++i) {
        is >> shipSizes[i];
    }

    auto playerShips = std::make_unique<ShipManager>(shipSizes);
    auto computerShips = std::make_unique<ShipManager>(shipSizes);
    state.loadShipManager(is, *playerShips);
    state.loadShipManager(is, *computerShips);

    auto playerField = std::make_unique<GameField>(fieldWidth, fieldHeight);
    auto computerField = std::make_unique<GameField>(fieldWidth, fieldHeight);
    state.loadGameField(is, *playerField, playerShips->getShips());
    state.loadGameField(is, *computerField, computerShips->getShips());

    auto playerAbilities = std::make_unique<AbilityManager>();
    state.loadAbilityManager(is, *playerAbilities);

    state.game->setGameStatus(static_cast<GameStatus>(stateInt));
    state.game->setCurrentRound(currentRound);
    state.game->updateGameComponents(
        std::move(playerField), std::move(computerField),
        std::move(playerShips), std::move(computerShips),
        std::move(playerAbilities)
    );

    return is;
}