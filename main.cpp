#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <filesystem>
#include <vector>
#include <iomanip>
#include "mainElements/game.h"
#include "mainElements/gamestate.h"

namespace fs = std::filesystem;

std::string getSaveFilePath(const std::string& filename) {
    return filename + ".txt";
}

std::vector<std::string> listSaveFiles() {
    std::vector<std::string> saves;
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".txt") {
            std::string filename = entry.path().stem().string();
            saves.push_back(filename);
        }
    }
    return saves;
}

void displaySaves() {
    auto saves = listSaveFiles();
    if (saves.empty()) {
        std::cout << "No saved games found.\n";
        return;
    }

    std::cout << "Available saves:\n";
    for (size_t i = 0; i < saves.size(); ++i) {
        std::cout << std::setw(3) << (i + 1) << ". " << saves[i] << '\n';
    }
}

std::string selectSaveFile(bool isLoading) {
    displaySaves();
    
    if (isLoading) {
        if (listSaveFiles().empty()) {
            throw std::runtime_error("No saved games found.");
        }
        
        std::cout << "\nEnter the number of the save to load or 'back' to return: ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "back") {
            return "";
        }
        
        try {
            int index = std::stoi(input) - 1;
            auto saves = listSaveFiles();
            if (index >= 0 && index < static_cast<int>(saves.size())) {
                return saves[index];
            }
        } catch (...) {
            throw std::runtime_error("Invalid save number.");
        }
        throw std::runtime_error("Invalid save number.");
    } else {
        std::cout << "\nEnter name for the save file or 'back' to return: ";
        std::string filename;
        std::getline(std::cin, filename);
        
        if (filename == "back") {
            return "";
        }

        if (filename.find_first_of("\\/:*?\"<>|") != std::string::npos) {
            throw std::runtime_error("Filename contains invalid characters.");
        }
        
        return filename;
    }
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayHelp() {
    std::cout << "\nAvailable commands:\n"
              << "  attack x y   - Attack position (x,y)\n"
              << "  ability      - Use current ability\n"
              << "  save         - Save current game\n"
              << "  load         - Load saved game\n"
              << "  saves        - List saved games\n"
              << "  display      - Show game fields\n"
              << "  help         - Show this help\n"
              << "  quit         - Exit game\n";
}

int main() {
    Game game;
    std::string input;
    const int MAX_ROUNDS = 3;

    std::cout << "Welcome to Battleship!\n";
    std::cout << "Enter 'new' to start a new game or 'load' to load a saved game: ";
    std::getline(std::cin, input);

    try {
        if (input == "load") {
            try {
                std::string filename = selectSaveFile(true);
                if (filename.empty()) {
                    game.startNewGame();
                } else {
                    GameState::loadGame(game, getSaveFilePath(filename));
                    std::cout << "Game loaded successfully.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Failed to load game: " << e.what() << "\nStarting a new game instead.\n";
                game.startNewGame();
            }
        } else {
            game.startNewGame();
        }

        displayHelp();

        while (true) {
            while (!game.isGameOver()) {
                std::cout << "\nCurrent Round: " << game.getCurrentRound() << "\n";
                
                if (game.hasPlayerAbility()) {
                    std::cout << "Available ability: " << game.getCurrentPlayerAbilityName() << "\n";
                }

                std::cout << "Enter command: ";
                if (!std::getline(std::cin, input)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));
                input.erase(input.find_last_not_of(" \t\n\r\f\v") + 1);

                if (input.empty()) continue;

                if (input == "help") {
                    displayHelp();
                } 
                else if (input == "quit") {
                    std::cout << "Do you want to save the game before quitting? (yes/no): ";
                    std::getline(std::cin, input);
                    if (input == "yes" || input == "y") {
                        try {
                            std::string filename = selectSaveFile(false);
                            if (!filename.empty()) {
                                GameState::saveGame(game, getSaveFilePath(filename));
                                std::cout << "Game saved.\n";
                            }
                        } catch (const std::exception& e) {
                            std::cout << "Failed to save game: " << e.what() << "\n";
                        }
                    }
                    return 0;
                } 
                else if (input == "display") {
                    clearScreen();
                    std::cout << "Your Field:\n";
                    game.displayField(game, true);
                    std::cout << "\nEnemy Field:\n";
                    game.displayField(game, false);
                    game.displayLegend();
                }
                else if (input == "saves") {
                    displaySaves();
                }
                else if (input == "save") {
                    try {
                        std::string filename = selectSaveFile(false);
                        if (!filename.empty()) {
                            GameState::saveGame(game, getSaveFilePath(filename));
                            std::cout << "Game saved successfully.\n";
                        }
                    } catch (const std::exception& e) {
                        std::cout << "Failed to save game: " << e.what() << "\n";
                    }
                } 
                else if (input == "load") {
                    try {
                        std::string filename = selectSaveFile(true);
                        if (!filename.empty()) {
                            GameState::loadGame(game, getSaveFilePath(filename));
                            std::cout << "Game loaded successfully.\n";
                        }
                    } catch (const std::exception& e) {
                        std::cout << "Failed to load game: " << e.what() << "\n";
                    }
                } 
                else if (input == "ability") {
                    if (!game.hasPlayerAbility()) {
                        std::cout << "No ability available.\n";
                        continue;
                    }
                    game.usePlayerAbility();
                } 
                else if (input.rfind("attack", 0) == 0) {
                    std::istringstream iss(input);
                    std::string command;
                    int x, y;
                    iss >> command >> x >> y;
                    
                    if (iss.fail()) {
                        std::cout << "Invalid attack format. Use: attack x y\n";
                        continue;
                    }

                    if (game.makePlayerAttack(x, y)) {
                        if (!game.isGameOver()) {
                            std::cout << "\nComputer's turn:\n";
                            game.makeComputerMove();
                        }
                    } else {
                        std::cout << "Invalid attack coordinates or cell already targeted.\n";
                    }
                } 
                else {
                    std::cout << "Unknown command. Type 'help' for available commands.\n";
                }
            }

            if (game.getGameStatus() == GameStatus::PlayerWon) {
                if (game.getCurrentRound() < MAX_ROUNDS) {
                    std::cout << "\nCongratulations! You won round " << game.getCurrentRound() << "!\n";
                    game.startNextRound();
                    continue;
                } else {
                    std::cout << "\nCongratulations! You've completed all " << MAX_ROUNDS << " rounds!\n";
                    break;
                }
            } else if (game.getGameStatus() == GameStatus::PlayerLost) {
                std::cout << "\nGame Over! You lost in round " << game.getCurrentRound() << ".\n";
                break;
            }
        }

        std::cout << "Would you like to start a new game? (yes/no): ";
        std::getline(std::cin, input);
        if (input == "yes" || input == "y") {
            main();
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}