#pragma once
#include "Card.hpp"
#include "../Entities/Entity.hpp"
#include <iostream>

namespace DeckBuilder {
    class PureAdrenalineCard : public Card {
    public:
        PureAdrenalineCard(std::string cardName, int cardCost, int cardValue) 
            : Card(std::move(cardName), cardCost, cardValue) {}

        void play(Entity* caster, Entity* /*target*/) override {
            std::cout << "\n> " << caster->getName() << " floods their system with " << name << "!" << std::endl;
            if (caster) {
                caster->applyStatus("Pure Adrenaline", value); 
                std::cout << "(All attacks and blocks gain +" << value << " output this turn!)" << std::endl;
            }
        }
    };
}