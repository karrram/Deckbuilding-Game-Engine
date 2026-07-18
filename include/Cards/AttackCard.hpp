#pragma once
#include "Card.hpp"
#include "../Entities/Entity.hpp" 
#include <iostream>

namespace DeckBuilder {
    class AttackCard : public Card {
    public:
        AttackCard(std::string cardName, int cardCost, int cardValue) 
            : Card(std::move(cardName), cardCost, cardValue) {}

        void play(Entity* caster, Entity* target) override {
            int totalDamage = value;
            
            if (caster) {
                totalDamage += caster->getStatus("Progressive Overload");
                totalDamage += caster->getStatus("Pure Adrenaline");
            }
            
            std::cout << "\n> " << caster->getName() << " played " << name << "!" << std::endl;
            
            if (target) {
                target->takeDamage(totalDamage); 
            }
            
            if (caster) {
                // Playing an attack stacks your multiplier for the NEXT attack
                caster->applyStatus("Progressive Overload", 2); 
            }
        }
    };
}