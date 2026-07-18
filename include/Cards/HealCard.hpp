#pragma once
#include "Card.hpp"
#include "../Entities/Entity.hpp"
#include <iostream>

namespace DeckBuilder {
    class HealCard : public Card {
    public:
        HealCard(std::string cardName, int cardCost, int cardValue) 
            : Card(std::move(cardName), cardCost, cardValue) {}

        void play(Entity* caster, Entity* /*target*/) override {
            std::cout << "\n> " << caster->getName() << " played " << name << "!" << std::endl;
            if (caster) {
                caster->heal(value); 
            }
        }
    };
}