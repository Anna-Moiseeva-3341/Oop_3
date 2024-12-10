#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "computerplayer.h"
#include "gamefield.h"
#include "shipmanager.h"
#include "../abilities/abilityManager.h"
#include <memory>
#include <iomanip>

enum class GameStatus {
    NotStarted,
    InProgress,
    PlayerWon,
    PlayerLost
};

class Game {
private:
    std::unique_ptr<GameField> playerField;
    std::unique_ptr<GameField> computerField;
    std::unique_ptr<ShipManager> playerShips;
    std::unique_ptr<ShipManager> computerShips;
    std::unique_ptr<Player> player;
    std::unique_ptr<ComputerPlayer> computer;
    std::unique_ptr<AbilityManager> playerAbilities;
    GameStatus state;
    int currentRound;
    int fieldWidth;
    int fieldHeight;
    std::vector<int> shipSizes;
    void initializeNewRound(bool keepPlayerState);
    void transferPlayerState(GameField& oldField, AbilityManager& oldAbilities,
                           GameField& newField, AbilityManager& newAbilities);
    bool placeShipRandomly(GameField& field, Ship& ship);
    bool placeAllShipsRandomly(GameField& field, ShipManager& manager);

public:
    Game(int width = 10, int height = 10);
    const ShipManager* getPlayerShips() const { return playerShips.get(); }
    const ShipManager* getComputerShips() const { return computerShips.get(); }
    const GameField* getPlayerField() const { return playerField.get(); }
    const GameField* getComputerField() const { return computerField.get(); }
    const AbilityManager* getPlayerAbilities() const { return playerAbilities.get(); }
    const std::vector<int>& getShipSizes() const { return shipSizes; }
    void setGameStatus(GameStatus status) { state = status; }
    void setCurrentRound(int round) { currentRound = round; }

    void updateGameComponents(
        std::unique_ptr<GameField> pField,
        std::unique_ptr<GameField> cField,
        std::unique_ptr<ShipManager> pShips,
        std::unique_ptr<ShipManager> cShips,
        std::unique_ptr<AbilityManager> pAbilities
    );
    void startNewGame();
    void startNextRound();
    bool usePlayerAbility();
    bool makePlayerAttack(int x, int y);
    bool makeComputerMove();
    GameStatus getGameStatus() const;
    int getCurrentRound() const;
    bool hasPlayerAbility() const;
    std::string getCurrentPlayerAbilityName() const;
    CellStatus getPlayerFieldStatus(int x, int y) const;
    CellStatus getComputerFieldStatus(int x, int y) const;
    bool isGameOver() const;
    int getFieldWidth() const { return fieldWidth; }
    int getFieldHeight() const { return fieldHeight; }
    void displayField(const Game& game, bool isPlayerField) const;
    void displayLegend() const;
    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);
    bool canSaveGame() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif