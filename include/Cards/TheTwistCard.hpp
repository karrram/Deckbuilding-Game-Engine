#pragma once
#include "Card.hpp"
#include "../Entities/Entity.hpp"
#include <iostream>

namespace DeckBuilder {
    class TheTwistCard : public Card {
    public:
        TheTwistCard(std::string cardName, int cardCost, int cardValue) 
            : Card(std::move(cardName), cardCost, cardValue) {}

        void play(Entity* caster, Entity* target) override {
            std::cout << "\n> " << caster->getName() << " attempts to unleash " << name << "!" << std::endl;
            
            if (caster->getStatus("Twist Charge") >= 3) {
                std::cout << ">>> THE TWIST IS EXECUTED! <<<" << std::endl;
                caster->reduceStatus("Twist Charge", 3); // Consume the 3 stacks
                if (target) {
                    target->takeDamage(value); 
                }
            } else {
                std::cout << "Not enough Twist Charges! You need 3, but only have " 
                          << caster->getStatus("Twist Charge") << ". The attack fizzles out!" << std::endl;
            }
        }
    };
}