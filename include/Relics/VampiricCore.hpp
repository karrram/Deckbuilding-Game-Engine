#pragma once
#include "Relic.hpp"
#include "../Entities/Entity.hpp"
#include "../Core/ConsoleColors.hpp"
#include <iostream>

namespace DeckBuilder {
    class VampiricCore : public Relic {
    public:
        VampiricCore() : Relic("Vampiric Core", "Heal 2 HP whenever you play an Attack card.") {}

        void onAttackPlayed(Entity* owner) override {
            if (owner) {
                std::cout << MAGENTA << "\n[Relic: Vampiric Core] activates! Siphoning life force..." << RESET << std::endl;
                owner->heal(2);
            }
        }
    };
}