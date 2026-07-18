#pragma once
#include "Entity.hpp"
#include "../Core/Deck.hpp"
#include "../Relics/Relic.hpp"
#include "../Core/ConsoleColors.hpp"
#include <vector>
#include <memory>
#include <iostream>

namespace DeckBuilder {
    class Player : public Entity {
    protected:
        int energy;
        int maxEnergy;
        std::vector<std::unique_ptr<Card>> hand;
        
        // The Relic Inventory
        std::vector<std::unique_ptr<Relic>> relics;

    public:
        Player(std::string playerName, int maxHealth, int maxEn) 
            : Entity(std::move(playerName), maxHealth), energy(maxEn), maxEnergy(maxEn) {}

        // --- RELIC LOGIC ---
        void addRelic(std::unique_ptr<Relic> newRelic) {
            std::cout << "\n" << BOLD << MAGENTA << ">>> Acquired Relic: [" << newRelic->getName() << "] - " 
                      << newRelic->getDescription() << " <<<" << RESET << std::endl;
            relics.push_back(std::move(newRelic));
        }

        void triggerOnAttackPlayed() override {
            for (auto& relic : relics) {
                relic->onAttackPlayed(this);
            }
        }

        // --- CORE PLAYER LOGIC ---
        void resetEnergy() { 
            energy = maxEnergy; 
        }

        int getEnergy() const { 
            return energy; 
        }

        size_t getHandSize() const { 
            return hand.size(); 
        }

        void drawCards(Deck& deck, int count) {
            for (int i = 0; i < count; ++i) {
                std::unique_ptr<Card> drawnCard = deck.drawCard();
                if (drawnCard) {
                    hand.push_back(std::move(drawnCard));
                } else {
                    std::cout << YELLOW << name << "'s draw pile is empty! Reshuffling..." << RESET << std::endl;
                    deck.shuffle();
                    // Try drawing one more time after reshuffling
                    drawnCard = deck.drawCard();
                    if (drawnCard) {
                        hand.push_back(std::move(drawnCard));
                    }
                }
            }
        }

        void showHand() const {
            if (hand.empty()) {
                std::cout << "Hand is empty." << std::endl;
                return;
            }
            std::cout << "--- " << name << "'s Hand ---" << std::endl;
            for (size_t i = 0; i < hand.size(); ++i) {
                std::cout << "[" << i << "] " << hand[i]->getName() 
                          << " (Cost: " << hand[i]->getCost() << ")" << std::endl;
            }
        }

        bool playCard(size_t index, Entity* target) {
            if (index >= hand.size()) {
                std::cout << RED << "Invalid card selection." << RESET << std::endl;
                return false;
            }

            if (energy >= hand[index]->getCost()) {
                energy -= hand[index]->getCost();
                
                // Play the card, then remove it from hand
                hand[index]->play(this, target);
                hand.erase(hand.begin() + index);
                return true;
            } else {
                std::cout << RED << "Not enough energy to play " << hand[index]->getName() << "!" << RESET << std::endl;
                return false;
            }
        }

        void discardHand() {
            hand.clear();
        }
    };
}