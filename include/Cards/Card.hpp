#pragma once
#include <string>
#include <utility>

namespace DeckBuilder {
    class Entity; // Forward declaration 

    class Card {
    protected:
        std::string name;
        int cost;
        int value;

    public:
        Card(std::string cardName, int cardCost, int cardValue) 
            : name(std::move(cardName)), cost(cardCost), value(cardValue) {}
        
        virtual ~Card() = default;

        std::string getName() const { return name; }
        int getCost() const { return cost; }
        int getValue() const { return value; }
        
        // It must have both 'caster' and 'target'
        virtual void play(Entity* caster, Entity* target) = 0; 
    };
}