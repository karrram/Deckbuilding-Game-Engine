#pragma once
#include "Entity.hpp"
#include <iostream>
#include <random>

namespace DeckBuilder {
    // Defines the different actions the AI can choose from
    enum class Intent {
        ATTACK,
        DEFEND
    };

    class Enemy : public Entity {
    private:
        int baseDamage;
        int intentValue;
        Intent currentIntent;
        std::mt19937 rng; // Advanced C++ random number generator

    public:
        Enemy(std::string enemyName, int maxHealth, int startingDamage) 
            : Entity(std::move(enemyName), maxHealth), baseDamage(startingDamage), intentValue(startingDamage), currentIntent(Intent::ATTACK) {
            
            // Seed the random number generator
            std::random_device rd;
            rng = std::mt19937(rd());
        }

        void generateNewIntent() {
            // 33% chance to Defend, 66% chance to Attack
            std::uniform_int_distribution<int> actionDist(0, 2);
            int roll = actionDist(rng);

            if (roll == 0) {
                currentIntent = Intent::DEFEND;
                // Generate a block value slightly higher than base damage
                std::uniform_int_distribution<int> blockDist(0, 4);
                intentValue = baseDamage + blockDist(rng);
            } else {
                currentIntent = Intent::ATTACK;
                // Add a slight variance (-2 to +2) to their attack damage
                std::uniform_int_distribution<int> dmgDist(-2, 2);
                intentValue = baseDamage + dmgDist(rng);
                if (intentValue < 0) intentValue = 0; // Prevent negative damage
            }
        }

        void showIntent() const {
            if (currentIntent == Intent::ATTACK) {
                std::cout << name << " intends to ATTACK for " << intentValue << " damage!" << std::endl;
            } else if (currentIntent == Intent::DEFEND) {
                std::cout << name << " intends to DEFEND and gain " << intentValue << " block!" << std::endl;
            }
        }

        Intent getIntent() const { return currentIntent; }
        int getIntentValue() const { return intentValue; }
    };
}