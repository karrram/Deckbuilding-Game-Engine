#pragma once
#include "../Cards/Card.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

namespace DeckBuilder {
    class Deck {
    private:
        // A vector holding unique pointers to our cards. 
        // This means the vector owns the memory, preventing leaks.
        std::vector<std::unique_ptr<Card>> cards;

    public:
        Deck() = default;
        ~Deck() = default;

        // Add a card to the deck (transfers ownership via std::move)
        void addCard(std::unique_ptr<Card> card);

        // Remove and return the top card
        std::unique_ptr<Card> drawCard();

        // Randomize the order of the cards
        void shuffle();

        // Check if empty
        bool isEmpty() const;
        
        // Get current deck size
        size_t getSize() const;
    };
}