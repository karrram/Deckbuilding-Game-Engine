#pragma once
#include <string>

namespace DeckBuilder {
    class Entity; // Forward declaration

    class Relic {
    protected:
        std::string name;
        std::string description;

    public:
        Relic(std::string relicName, std::string relicDesc) 
            : name(std::move(relicName)), description(std::move(relicDesc)) {}
        
        virtual ~Relic() = default;

        std::string getName() const { return name; }
        std::string getDescription() const { return description; }

        // Hooks that the game can trigger
        virtual void onCombatStart(Entity* /*owner*/) {}
        virtual void onTurnStart(Entity* /*owner*/) {}
        virtual void onAttackPlayed(Entity* /*owner*/) {} 
    };
}