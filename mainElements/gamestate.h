#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include "mainElements/game.h"
#include "mainElements/gamefield.h"
#include "mainElements/ship.h"
#include "mainElements/shipmanager.h"
#include "abilities/abilityManager.h"

class GameState {
public:
    static void saveGame(const Game& game, const std::string& filename);
    static void loadGame(Game& game, const std::string& filename);

    friend std::ostream& operator<<(std::ostream& os, const GameState& state);
    friend std::istream& operator>>(std::istream& is, GameState& state);

private:
    Game* game{nullptr}; 
    void saveShipManager(std::ostream& os, const ShipManager& manager) const;
    void loadShipManager(std::istream& is, ShipManager& manager);
    
    void saveGameField(std::ostream& os, const GameField& field, 
                      const std::vector<Ship>& ships) const;
    void loadGameField(std::istream& is, GameField& field,
                      std::vector<Ship>& ships);
    
    void saveAbilityManager(std::ostream& os, const AbilityManager& manager) const;
    void loadAbilityManager(std::istream& is, AbilityManager& manager);
    std::map<const Ship*, int> createShipPtrToIndexMap(const std::vector<Ship>& ships) const;
    explicit GameState(Game* game = nullptr) : game(game) {}
};

std::ostream& operator<<(std::ostream& os, const GameState& state);
std::istream& operator>>(std::istream& is, GameState& state);

#endif
