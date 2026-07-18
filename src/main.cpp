#include <iostream>
#include <string>
#include <vector>
#include "Entities/Player.hpp"
#include "Entities/Enemy.hpp"
#include "Core/Deck.hpp"
#include "Core/CardFactory.hpp"
#include "Core/ConsoleColors.hpp"

using namespace DeckBuilder;

enum class GameState {
    PLAYER_TURN,
    ENEMY_TURN,
    COMBAT_OVER,
    GAME_OVER
};

int main() {
    std::cout << CYAN << "--- DeckBuilder Engine Initialized ---" << RESET << std::endl;

    CardFactory factory;
    if (!factory.loadDatabase("data/cards.csv")) {
        return -1; 
    }

    // The persistent Master Deck. This grows as you draft new cards!
    std::vector<std::string> masterDeck = {
        "Strike", "Strike", 
        "Heavy Blow", "Heavy Blow", 
        "Iron Core", 
        "Active Recovery", 
        "Pure Adrenaline", 
        "The Twist"
    };

    Player hero("Karam", 50, 3);
    int floorNumber = 1;
    bool isAlive = true;

    // --- THE DUNGEON LOOP ---
    while (isAlive) {
        std::cout << "\n" << BOLD << MAGENTA << "=== DUNGEON FLOOR " << floorNumber << " ===" << RESET << std::endl;

        // 1. Build a fresh deck from the Master Deck for this encounter
        Deck combatDeck;
        for (const auto& cardName : masterDeck) {
            combatDeck.addCard(factory.createCard(cardName));
        }
        combatDeck.shuffle();

        // 2. Procedurally generate a harder boss for each floor
        int bossHp = 80 + (floorNumber * 20);
        int bossDmg = 6 + (floorNumber * 2);
        std::string bossName = "Sentinel Mk-" + std::to_string(floorNumber);
        Enemy boss(bossName, bossHp, bossDmg);

        GameState currentState = GameState::PLAYER_TURN;
        int roundNumber = 1;

        std::cout << BOLD << RED << ">>> " << boss.getName() << " blocks your path! <<<" << RESET << std::endl;
        boss.generateNewIntent();
        boss.showIntent();

        // --- THE COMBAT LOOP ---
        while (currentState != GameState::COMBAT_OVER && currentState != GameState::GAME_OVER) {

            if (currentState == GameState::PLAYER_TURN) {
                std::cout << "\n" << BOLD << YELLOW << "--- ROUND " << roundNumber << " | " << hero.getName() << "'s Turn ---" << RESET << std::endl;
                hero.resetBlock();
                hero.resetEnergy(); 
                hero.drawCards(combatDeck, 3); 
                
                bool isTurnActive = true;
                while (isTurnActive) {
                    std::cout << "\n" << GREEN << "[HP: " << hero.getHp() << "]" << RESET << " | " 
                              << CYAN << "[Block: " << hero.getBlock() << "]" << RESET << " | " 
                              << YELLOW << "[Energy: " << hero.getEnergy() << "]" << RESET << std::endl;
                    hero.showHand();
                    
                    std::cout << "\nEnter card number to play, 'A' for Armor, or 'E' to End Turn: ";
                    std::string input;
                    std::cin >> input;

                    if (input == "A" || input == "a" || input == "E" || input == "e") {
                        if (input == "A" || input == "a") {
                            std::cout << "\n> " << hero.getName() << " sacrifices their turn to brace for impact!" << std::endl;
                            hero.addBlock(20);
                        } else {
                            std::cout << hero.getName() << " ends their turn." << std::endl;
                        }

                        // --- END OF TURN LOGIC ---
                        if (boss.getTurnDamageTaken() >= 20) {
                            std::cout << YELLOW << "\n>>> The sheer volume of damage leaves the boss FATIGUED! <<<" << RESET << std::endl;
                            boss.applyStatus("Fatigued", 2); 
                        }
                        
                        boss.resetTurnDamage();
                        hero.clearStatus("Progressive Overload");
                        hero.clearStatus("Pure Adrenaline");
                        
                        hero.discardHand(); 
                        isTurnActive = false;
                        currentState = GameState::ENEMY_TURN; 
                    } 
                    else {
                        try {
                            size_t cardIndex = std::stoi(input);
                            if (hero.playCard(cardIndex, &boss)) {
                                if (boss.isDead()) {
                                    std::cout << BOLD << GREEN << "\n>>> You defeated " << boss.getName() << "! <<<" << RESET << std::endl;
                                    isTurnActive = false;
                                    currentState = GameState::COMBAT_OVER; 
                                }
                            }
                        } catch (...) {
                            std::cout << RED << "Invalid input." << RESET << std::endl;
                        }
                    }
                }
            } 
            
            else if (currentState == GameState::ENEMY_TURN) {
                std::cout << "\n" << BOLD << RED << "--- " << boss.getName() << "'s Turn ---" << RESET << std::endl;
                boss.resetBlock(); 
                
                if (boss.getIntent() == Intent::ATTACK) {
                    int fatigueDebuff = boss.getStatus("Fatigued");
                    int finalDamage = boss.getIntentValue() - fatigueDebuff;
                    if (finalDamage < 0) finalDamage = 0;

                    if (fatigueDebuff > 0) {
                        std::cout << boss.getName() << " attacks for " << finalDamage << " damage! (Reduced by " << fatigueDebuff << " Fatigue)" << std::endl;
                    } else {
                        std::cout << boss.getName() << " attacks for " << finalDamage << " damage!" << std::endl;
                    }
                    hero.takeDamage(finalDamage);
                } 
                else if (boss.getIntent() == Intent::DEFEND) {
                    boss.addBlock(boss.getIntentValue());
                }

                if (hero.isDead()) {
                    std::cout << BOLD << RED << "\n" << hero.getName() << " has perished..." << RESET << std::endl;
                    isAlive = false;
                    currentState = GameState::GAME_OVER; 
                } else {
                    boss.generateNewIntent(); 
                    boss.showIntent();
                    roundNumber++;
                    currentState = GameState::PLAYER_TURN; 
                }
            }
        }

        // --- POST-COMBAT RESOLUTION ---
        if (isAlive) {
            std::cout << GREEN << "You catch your breath, recovering 15 HP." << RESET << std::endl;
            hero.heal(15);
            // Clear combat-specific statuses between floors
            hero.clearStatus("Twist Charge"); 

            // --- CARD REWARD DRAFT ---
            std::cout << "\n" << BOLD << YELLOW << "=== CHOOSE YOUR REWARD ===" << RESET << std::endl;
            std::vector<std::string> draftOptions = factory.getRandomCardNames(3);

            for (size_t i = 0; i < draftOptions.size(); ++i) {
                std::cout << "[" << i + 1 << "] " << draftOptions[i] << std::endl;
            }
            std::cout << "[0] Skip Reward" << std::endl;

            bool validChoice = false;
            while (!validChoice) {
                std::cout << "Enter your choice (0-" << draftOptions.size() << "): ";
                std::string draftInput;
                std::cin >> draftInput;

                try {
                    int choice = std::stoi(draftInput);
                    if (choice == 0) {
                        std::cout << "You chose to skip the reward." << std::endl;
                        validChoice = true;
                    } else if (choice > 0 && (size_t)choice <= draftOptions.size()) {
                        std::string chosenCard = draftOptions[choice - 1];
                        
                        // Push the new card into your persistent deck!
                        masterDeck.push_back(chosenCard);
                        std::cout << BOLD << GREEN << ">>> Added " << chosenCard << " to your Master Deck! <<<" << RESET << std::endl;
                        validChoice = true;
                    } else {
                        std::cout << RED << "Invalid choice." << RESET << std::endl;
                    }
                } catch (...) {
                    std::cout << RED << "Invalid input. Please enter a valid number." << RESET << std::endl;
                }
            }

            floorNumber++;
        }
    }

    std::cout << "\n" << BOLD << RED << "=== RUN ENDED ON FLOOR " << floorNumber << " ===" << RESET << std::endl;
    return 0;
}