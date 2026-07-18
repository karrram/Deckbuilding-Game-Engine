#pragma once
#include "Card.hpp"
#include "../Entities/Entity.hpp"
#include <iostream>

namespace DeckBuilder {
    class DefenseCard : public Card {
    public:
        DefenseCard(std::string cardName, int cardCost, int cardValue) 
            : Card(std::move(cardName), cardCost, cardValue) {}

        void play(Entity* caster, Entity* /*target*/) override {
            int totalBlock = value;
            
            if (caster) {
                totalBlock += caster->getStatus("Pure Adrenaline");
            }
            
            std::cout << "\n> " << caster->getName() << " played " << name << "!" << std::endl;
            
            if (caster) {
                caster->addBlock(totalBlock); 
            }
        }
    };
}